import qbs.base 1.0

import "../QtcPlugin.qbs" as QtcPlugin

QtcPlugin {
    name: "TextEditor"

    Depends { name: "qt"; submodules: ['widgets', 'xml', 'network', 'script'] }
    Depends { name: "Find" }
    Depends { name: "Locator" }
    Depends { name: "Core" }
    Depends { name: "cpp" }

    cpp.includePaths: [
        ".",
        "..",
        "generichighlighter",
        "tooltip",
        "snippets",
        "codeassist",
        "../../libs",
        buildDirectory
    ]

    files: [
        "TextEditor.mimetypes.xml",
        "colorschemeedit.ui",
        "fontsettingspage.ui",
        "texteditor.qrc",
        "autocompleter.cpp",
        "autocompleter.h",
        "basefilefind.cpp",
        "basefilefind.h",
        "basefilefind_p.h",
        "basehoverhandler.cpp",
        "basehoverhandler.h",
        "basetextdocument.cpp",
        "basetextdocument.h",
        "basetextdocumentlayout.cpp",
        "basetextdocumentlayout.h",
        "basetexteditor.cpp",
        "basetexteditor.h",
        "basetexteditor_p.h",
        "basetextmark.cpp",
        "basetextmark.h",
        "behaviorsettings.cpp",
        "behaviorsettings.h",
        "behaviorsettingspage.cpp",
        "behaviorsettingspage.h",
        "behaviorsettingspage.ui",
        "behaviorsettingswidget.cpp",
        "behaviorsettingswidget.h",
        "behaviorsettingswidget.ui",
        "circularclipboard.h",
        "codecselector.cpp",
        "codecselector.h",
        "codestyleeditor.cpp",
        "codestylepool.cpp",
        "codestylepool.h",
        "codestyleselectorwidget.cpp",
        "codestyleselectorwidget.h",
        "codestyleselectorwidget.ui",
        "colorscheme.cpp",
        "colorscheme.h",
        "colorschemeedit.cpp",
        "colorschemeedit.h",
        "completionsettings.cpp",
        "completionsettings.h",
        "convenience.cpp",
        "convenience.h",
        "displaysettings.cpp",
        "displaysettings.h",
        "displaysettingspage.cpp",
        "displaysettingspage.h",
        "displaysettingspage.ui",
        "extraencodingsettings.cpp",
        "extraencodingsettings.h",
        "findincurrentfile.cpp",
        "findincurrentfile.h",
        "findinfiles.cpp",
        "findinfiles.h",
        "findinopenfiles.cpp",
        "findinopenfiles.h",
        "fontsettings.cpp",
        "fontsettings.h",
        "fontsettingspage.cpp",
        "fontsettingspage.h",
        "helpitem.cpp",
        "helpitem.h",
        "icodestylepreferences.cpp",
        "icodestylepreferencesfactory.cpp",
        "icodestylepreferencesfactory.h",
        "indenter.cpp",
        "indenter.h",
        "ioutlinewidget.h",
        "itexteditor.cpp",
        "itexteditor.h",
        "itextmark.cpp",
        "itextmark.h",
        "linenumberfilter.cpp",
        "linenumberfilter.h",
        "normalindenter.cpp",
        "normalindenter.h",
        "outlinefactory.cpp",
        "outlinefactory.h",
        "plaintexteditor.cpp",
        "plaintexteditor.h",
        "plaintexteditorfactory.cpp",
        "plaintexteditorfactory.h",
        "quickfix.cpp",
        "quickfix.h",
        "refactoringchanges.cpp",
        "refactoringchanges.h",
        "refactoroverlay.cpp",
        "refactoroverlay.h",
        "semantichighlighter.cpp",
        "semantichighlighter.h",
        "simplecodestylepreferences.cpp",
        "simplecodestylepreferences.h",
        "simplecodestylepreferenceswidget.cpp",
        "storagesettings.cpp",
        "storagesettings.h",
        "syntaxhighlighter.cpp",
        "syntaxhighlighter.h",
        "tabsettings.cpp",
        "tabsettings.h",
        "tabsettingswidget.cpp",
        "tabsettingswidget.h",
        "tabsettingswidget.ui",
        "texteditor_global.h",
        "texteditoractionhandler.cpp",
        "texteditoractionhandler.h",
        "texteditorconstants.h",
        "texteditoroptionspage.cpp",
        "texteditoroptionspage.h",
        "texteditoroverlay.cpp",
        "texteditoroverlay.h",
        "texteditorplugin.cpp",
        "texteditorplugin.h",
        "texteditorsettings.cpp",
        "texteditorsettings.h",
        "textfilewizard.cpp",
        "textfilewizard.h",
        "typingsettings.cpp",
        "typingsettings.h",
        "circularclipboard.cpp",
        "codestyleeditor.h",
        "icodestylepreferences.h",
        "simplecodestylepreferenceswidget.h",
        "codeassist/assistenums.h",
        "codeassist/basicproposalitem.cpp",
        "codeassist/basicproposalitem.h",
        "codeassist/basicproposalitemlistmodel.cpp",
        "codeassist/basicproposalitemlistmodel.h",
        "codeassist/codeassistant.cpp",
        "codeassist/codeassistant.h",
        "codeassist/completionassistprovider.cpp",
        "codeassist/completionassistprovider.h",
        "codeassist/defaultassistinterface.cpp",
        "codeassist/defaultassistinterface.h",
        "codeassist/functionhintproposal.cpp",
        "codeassist/functionhintproposal.h",
        "codeassist/functionhintproposalwidget.cpp",
        "codeassist/functionhintproposalwidget.h",
        "codeassist/genericproposal.cpp",
        "codeassist/genericproposal.h",
        "codeassist/genericproposalwidget.cpp",
        "codeassist/genericproposalwidget.h",
        "codeassist/iassistinterface.cpp",
        "codeassist/iassistinterface.h",
        "codeassist/iassistprocessor.cpp",
        "codeassist/iassistprocessor.h",
        "codeassist/iassistproposal.cpp",
        "codeassist/iassistproposal.h",
        "codeassist/iassistproposalitem.cpp",
        "codeassist/iassistproposalitem.h",
        "codeassist/iassistproposalmodel.cpp",
        "codeassist/iassistproposalmodel.h",
        "codeassist/iassistproposalwidget.cpp",
        "codeassist/iassistproposalwidget.h",
        "codeassist/iassistprovider.cpp",
        "codeassist/iassistprovider.h",
        "codeassist/ifunctionhintproposalmodel.cpp",
        "codeassist/ifunctionhintproposalmodel.h",
        "codeassist/igenericproposalmodel.cpp",
        "codeassist/igenericproposalmodel.h",
        "codeassist/quickfixassistprocessor.cpp",
        "codeassist/quickfixassistprocessor.h",
        "codeassist/quickfixassistprovider.cpp",
        "codeassist/quickfixassistprovider.h",
        "codeassist/runner.cpp",
        "codeassist/runner.h",
        "generichighlighter/context.cpp",
        "generichighlighter/context.h",
        "generichighlighter/definitiondownloader.cpp",
        "generichighlighter/definitiondownloader.h",
        "generichighlighter/dynamicrule.cpp",
        "generichighlighter/dynamicrule.h",
        "generichighlighter/highlightdefinition.cpp",
        "generichighlighter/highlightdefinition.h",
        "generichighlighter/highlightdefinitionhandler.cpp",
        "generichighlighter/highlightdefinitionhandler.h",
        "generichighlighter/highlightdefinitionmetadata.cpp",
        "generichighlighter/highlightdefinitionmetadata.h",
        "generichighlighter/highlighter.cpp",
        "generichighlighter/highlighter.h",
        "generichighlighter/highlighterexception.h",
        "generichighlighter/highlightersettings.cpp",
        "generichighlighter/highlightersettings.h",
        "generichighlighter/highlightersettingspage.cpp",
        "generichighlighter/highlightersettingspage.h",
        "generichighlighter/highlightersettingspage.ui",
        "generichighlighter/includerulesinstruction.cpp",
        "generichighlighter/includerulesinstruction.h",
        "generichighlighter/itemdata.cpp",
        "generichighlighter/itemdata.h",
        "generichighlighter/keywordlist.cpp",
        "generichighlighter/keywordlist.h",
        "generichighlighter/managedefinitionsdialog.cpp",
        "generichighlighter/managedefinitionsdialog.h",
        "generichighlighter/managedefinitionsdialog.ui",
        "generichighlighter/manager.cpp",
        "generichighlighter/manager.h",
        "generichighlighter/progressdata.cpp",
        "generichighlighter/progressdata.h",
        "generichighlighter/reuse.h",
        "generichighlighter/rule.cpp",
        "generichighlighter/rule.h",
        "generichighlighter/specificrules.cpp",
        "generichighlighter/specificrules.h",
        "snippets/isnippetprovider.cpp",
        "snippets/isnippetprovider.h",
        "snippets/plaintextsnippetprovider.cpp",
        "snippets/plaintextsnippetprovider.h",
        "snippets/reuse.h",
        "snippets/snippet.cpp",
        "snippets/snippet.h",
        "snippets/snippetassistcollector.cpp",
        "snippets/snippetassistcollector.h",
        "snippets/snippeteditor.cpp",
        "snippets/snippeteditor.h",
        "snippets/snippetscollection.cpp",
        "snippets/snippetscollection.h",
        "snippets/snippetssettings.cpp",
        "snippets/snippetssettings.h",
        "snippets/snippetssettingspage.cpp",
        "snippets/snippetssettingspage.h",
        "snippets/snippetssettingspage.ui",
        "tooltip/effects.h",
        "tooltip/reuse.h",
        "tooltip/tipcontents.cpp",
        "tooltip/tipcontents.h",
        "tooltip/tipfactory.cpp",
        "tooltip/tipfactory.h",
        "tooltip/tips.cpp",
        "tooltip/tips.h",
        "tooltip/tooltip.cpp",
        "tooltip/tooltip.h"
    ]
    ProductModule {
        Depends { name: "Find" }
        Depends { name: "Locator" }
    }
}

