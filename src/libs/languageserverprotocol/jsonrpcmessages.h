/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include "icontent.h"
#include "lsptypes.h"
#include "jsonkeys.h"

#include <utils/optional.h>
#include <utils/qtcassert.h>
#include <utils/variant.h>

#include <QDebug>
#include <QHash>
#include <QJsonObject>
#include <QJsonValue>
#include <QCoreApplication>
#include <QUuid>

namespace LanguageServerProtocol {

using LanguageClientNull = JsonObject;

class LANGUAGESERVERPROTOCOL_EXPORT JsonRpcMessage : public IContent
{
public:
    JsonRpcMessage();
    JsonRpcMessage(const QJsonObject &jsonObject);
    JsonRpcMessage(QJsonObject &&jsonObject);

    QByteArray toRawData() const final;
    QByteArray mimeType() const final;
    bool isValid(QString *errorMessage) const override;

protected:
    QJsonObject m_jsonObject;

private:
    QString m_parseError;
};

class LANGUAGESERVERPROTOCOL_EXPORT JsonRpcMessageHandler
{
    Q_DECLARE_TR_FUNCTIONS(JsonRpcMessageHandler)

public:
    using MessageProvider = std::function<IContent *(const QJsonObject &)>;
    static void registerMessageProvider(const QString &method, MessageProvider provider);
    template<typename T>
    static void registerMessageProvider()
    {
        registerMessageProvider(T::methodName, [](const QJsonObject &object){
            return new T(object);
        });
    }
    static QByteArray jsonRpcMimeType();
    static void parseContent(const QByteArray &content, QTextCodec *codec, QString &errorMessage,
                             ResponseHandlers responseHandlers,
                             MethodHandler methodHandler);
    static QJsonObject toJsonObject(const QByteArray &content, QTextCodec *codec, QString &parseError);

private:
    static QHash<QString, MessageProvider> m_messageProvider;
};

template <typename Params>
class Notification : public JsonRpcMessage
{
public:
    Notification() : Notification(QString()) {}
    Notification(const QString &methodName, const Params &params = Params())
    {
        setMethod(methodName);
        setParams(params);
    }
    using JsonRpcMessage::JsonRpcMessage;

    QString method() const
    { return fromJsonValue<QString>(m_jsonObject.value(methodKey)); }
    void setMethod(const QString &method)
    { m_jsonObject.insert(methodKey, method); }

    Utils::optional<Params> params() const
    {
        const QJsonValue &params = m_jsonObject.value(parametersKey);
        return params.isUndefined() ? Utils::nullopt : Utils::make_optional(Params(params));
    }
    void setParams(const Params &params)
    { m_jsonObject.insert(parametersKey, QJsonValue(params)); }
    void clearParams() { m_jsonObject.remove(parametersKey); }

    bool isValid(QString *errorMessage) const override
    {
        return JsonRpcMessage::isValid(errorMessage)
                && m_jsonObject.value(methodKey).isString()
                && parametersAreValid(errorMessage);
    }

    virtual bool parametersAreValid(QString *errorMessage) const
    {
        if (auto parameter = params()) {
            QStringList error;
            return parameter.value().isValid(&error);
        }
        if (errorMessage)
            *errorMessage = QCoreApplication::translate("LanguageServerProtocol::Notification",
                                                        "No parameters in \"%1\".").arg(method());
        return false;
    }
};

template <typename Error>
class ResponseError : public JsonObject
{
public:
    using JsonObject::JsonObject;

    int code() const { return typedValue<int>(codeKey); }
    void setCode(int code) { insert(codeKey, code); }

    QString message() const { return typedValue<QString>(messageKey); }
    void setMessage(const QString &message) { insert(messageKey, message); }

    Utils::optional<Error> data() const { return optionalValue<Error>(dataKey); }
    void setData(const Error &data) { insert(dataKey, data); }
    void clearData() { remove(dataKey); }

    bool isValid(QStringList *error) const override
    {
        return check<int>(error, codeKey)
                && check<QString>(error, messageKey)
                && checkOptional<Error>(error, dataKey);
    }

    // predefined error codes
    enum ErrorCodes {
        // Defined by JSON RPC
        ParseError = -32700,
        InvalidRequest = -32600,
        MethodNotFound = -32601,
        InvalidParams = -32602,
        InternalError = -32603,
        serverErrorStart = -32099,
        serverErrorEnd = -32000,
        ServerNotInitialized = -32002,
        UnknownErrorCode = -32001,

        // Defined by the protocol.
        RequestCancelled = -32800
    };

#define CASE_ERRORCODES(x) case ErrorCodes::x: return QLatin1String(#x)
    static QString errorCodesToString(int code)
    {
        switch (code) {
        CASE_ERRORCODES(ParseError);
        CASE_ERRORCODES(InvalidRequest);
        CASE_ERRORCODES(MethodNotFound);
        CASE_ERRORCODES(InvalidParams);
        CASE_ERRORCODES(InternalError);
        CASE_ERRORCODES(serverErrorStart);
        CASE_ERRORCODES(serverErrorEnd);
        CASE_ERRORCODES(ServerNotInitialized);
        CASE_ERRORCODES(UnknownErrorCode);
        CASE_ERRORCODES(RequestCancelled);
        default:
            return QCoreApplication::translate("LanguageClient::ResponseError",
                                               "Error %1").arg(code);
        }
    }
#undef CASE_ERRORCODES
};

template <typename Result, typename Error>
class Response : public JsonRpcMessage
{
public:
    using JsonRpcMessage::JsonRpcMessage;

    MessageId id() const
    { return MessageId(m_jsonObject.value(idKey)); }
    void setId(MessageId id)
    { this->m_jsonObject.insert(idKey, id.toJson()); }

    Utils::optional<Result> result() const
    {
        const QJsonValue &result = m_jsonObject.value("result");
        if (result.isUndefined())
            return Utils::nullopt;
        return Utils::make_optional(Result(result));
    }
    void setResult(const Result &result) { m_jsonObject.insert(resultKey, result); }
    void clearResult() { m_jsonObject.remove(resultKey); }

    Utils::optional<ResponseError<Error>> error() const
    {
        const QJsonValue &val = m_jsonObject.value(errorKey);
        return val.isUndefined() ? Utils::nullopt
                                 : Utils::make_optional(fromJsonValue<ResponseError<Error>>(val));
    }
    void setError(const ResponseError<Error> &error)
    { m_jsonObject.insert(errorKey, QJsonValue(error)); }
    void clearError() { m_jsonObject.remove(errorKey); }

    bool isValid(QString *errorMessage) const override
    { return JsonRpcMessage::isValid(errorMessage) && id().isValid(); }
};

template <typename Result, typename Error, typename Params>
class Request : public Notification<Params>
{
public:
    Request() : Notification<Params>() { setId(QUuid::createUuid().toString()); }
    Request(const QString &methodName, const Params &params = Params())
        : Notification<Params>(methodName, params)
    { setId(QUuid::createUuid().toString()); }
    Request(const QJsonObject &jsonObject) : Notification<Params>(jsonObject) { }
    Request(QJsonObject &&jsonObject) : Notification<Params>(std::move(jsonObject)) { }

    MessageId id() const
    { return MessageId(JsonRpcMessage::m_jsonObject.value(idKey)); }
    void setId(const MessageId &id)
    { JsonRpcMessage::m_jsonObject.insert(idKey, id.toJson()); }

    using ResponseCallback = std::function<void(Response<Result, Error>)>;
    void setResponseCallback(const ResponseCallback &callback)
    { m_callBack = callback; }

    void registerResponseHandler(QHash<MessageId, ResponseHandler> *handlers) const final
    {
        auto callback = m_callBack;
        handlers->insert(id(), [callback](const QByteArray &content, QTextCodec *codec){
            if (!callback)
                return;
            QString parseError;
            const QJsonObject &object =
                    JsonRpcMessageHandler::toJsonObject(content, codec, parseError);
            Response<Result, Error> response(object);
            if (object.isEmpty()) {
                ResponseError<Error> error;
                error.setMessage(parseError);
                response.setError(ResponseError<Error>());
            }
            callback(Response<Result, Error>(object));
        });
    }

    bool isValid(QString *errorMessage) const override
    {
        if (!Notification<Params>::isValid(errorMessage))
            return false;
        if (id().isValid())
            return true;
        if (errorMessage) {
            *errorMessage = QCoreApplication::translate("LanguageServerProtocol::Request",
                                                        "No ID set in \"%1\".").arg(this->method());
        }
        return false;
    }

private:
    ResponseCallback m_callBack;
};

class LANGUAGESERVERPROTOCOL_EXPORT CancelParameter : public JsonObject
{
public:
    CancelParameter(const MessageId &id) { setId(id); }
    CancelParameter() = default;
    using JsonObject::JsonObject;

    MessageId id() const { return MessageId(value(idKey)); }
    void setId(const MessageId &id) { insert(idKey, id.toJson()); }

    bool isValid(QStringList *error) const override
    {
        if (MessageId(value(idKey)).isValid(error))
            return true;
        if (error)
            error->append(idKey);
        return false;
    }
};

class LANGUAGESERVERPROTOCOL_EXPORT CancelRequest : public Notification<CancelParameter>
{
public:
    CancelRequest(const CancelParameter &params = CancelParameter());
    using Notification::Notification;
    constexpr static const char methodName[] = "$/cancelRequest";
};

} // namespace LanguageClient

template <typename Error>
inline QDebug operator<<(QDebug stream,
                const LanguageServerProtocol::ResponseError<Error> &error)
{
    stream.nospace() << LanguageServerProtocol::ResponseError<Error>::errorCodesToString(error.code())
                     << ":"
                     << error.message();
    return stream;
}
