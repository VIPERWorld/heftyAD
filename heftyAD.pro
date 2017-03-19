#-------------------------------------------------
#
# Project created by QtCreator 2015-10-07T23:37:52
#
#-------------------------------------------------

CONFIG += c++11

QT       += core gui xml qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = heftyAD
TEMPLATE = app

unix {
    #LIBS += "/home/mrfad/Projects/Personnal/C++/qt_multislider/code/msSharedLibTester/ms_sharedlibs/libmsSharedLib.so"
    #LIBS += -L/home/mrfad/Projects/Personnal/C++/qt_multislider/code/msSharedLibTester/ms_sharedlibs
    #        -l/libmsSharedLib.so
}

win32 {
    CONFIG(release, debug|release) {
        LIBS += "D:\Perso\Qt\qt_heftyAD\heftyAD\tools\dynamiclibs\release\nicecpplib.dll"
        LIBS += "D:\Perso\Qt\qt_heftyAD\heftyAD\tools\dynamiclibs\release\qscintilla2.dll"
    } else {
        LIBS += "D:\Perso\Qt\qt_heftyAD\heftyAD\tools\dynamiclibs\debug\nicecpplibd.dll"
        LIBS += "D:\Perso\Qt\qt_heftyAD\heftyAD\tools\dynamiclibs\debug\qscintilla2.dll"
    }
    # do not forget to copy each "non-Qt dll" to the directory which contains the executable before running it from QtCreator.
}

RESOURCES += \
    heftyAD.qrc

INCLUDEPATH += \
    src-libs \
    src-libs/nicecpp \
    src-libs/nicecpp/pattern \
    src-libs/qscintilla \
    #
    src-niceqt \
    #
    src-heftyad \
    src-heftyad/algorithm \
    src-heftyad/algorithm/basis \
    src-heftyad/model \
    src-heftyad/model/animation \
    src-heftyad/model/inout \
    src-heftyad/model/inout/array \
    src-heftyad/model/inout/graph \
    src-heftyad/model/inout/helpers \
    src-heftyad/model/model \
    src-heftyad/model/model/array \
    src-heftyad/model/model/connectionarea \
    src-heftyad/model/model/graph \
    src-heftyad/model/view \
    src-heftyad/model/view/array \
    src-heftyad/model/view/connectionarea \
    src-heftyad/model/view/graph \
    src-heftyad/model/viewform \
    src-heftyad/model/viewform/item \
    src-heftyad/menu \
    src-heftyad/plugin \
    src-heftyad/plugin/form \
    src-heftyad/scintilla \
    src-heftyad/shadow \
    src-heftyad/simulation \
    src-heftyad/simulation/console \
    src-heftyad/work \
    src-heftyad/workcontent \
    src-heftyad/workcontent/algorithm \
    src-heftyad/workcontent/model \
    src-heftyad/workspace

HEADERS += \
    src-heftyad/algorithm/basis/Controllable.hpp \
    src-heftyad/algorithm/basis/Iterable.h \
    src-heftyad/algorithm/basis/SimplyControllable.h \
    src-heftyad/algorithm/Algorithm.h \
    src-heftyad/algorithm/AlgorithmRunner.h \
    src-heftyad/menu/EditMenu.h \
    src-heftyad/menu/FileMenu.h \
    src-heftyad/menu/SettingMenu.h \
    src-heftyad/model/animation/ModelPropertyAnimation.h \
    src-heftyad/model/inout/array/ArrayModelXmlReader.h \
    src-heftyad/model/inout/array/ArrayModelXmlWriter.h \
    src-heftyad/model/inout/graph/GraphModelXmlReader.h \
    src-heftyad/model/inout/graph/GraphModelXmlWriter.h \
    src-heftyad/model/inout/helpers/ModelReaderXmlHelper.h \
    src-heftyad/model/inout/helpers/ModelWriterXmlHelper.h \
    src-heftyad/model/inout/ModelReader.h \
    src-heftyad/model/inout/ModelWriter.h \
    src-heftyad/model/model/array/ArrayModel.h \
    src-heftyad/model/model/array/ArrayModelItem.h \
    src-heftyad/model/model/connectionarea/ModelCurve.h \
    src-heftyad/model/model/connectionarea/ModelNode.h \
    src-heftyad/model/model/graph/GraphModel.h \
    src-heftyad/model/model/graph/GraphModelEdge.h \
    src-heftyad/model/model/graph/GraphModelVertex.h \
    src-heftyad/model/model/Model.h \
    src-heftyad/model/model/ModelItem.h \
    src-heftyad/model/model/ModelMultiShapeItem.h \
    src-heftyad/model/view/array/ArrayView.h \
    src-heftyad/model/view/array/ArrayViewItem.h \
    src-heftyad/model/view/connectionarea/ViewCurve.h \
    src-heftyad/model/view/connectionarea/ViewCurvePoint.h \
    src-heftyad/model/view/connectionarea/ViewNode.h \
    src-heftyad/model/view/graph/GraphView.h \
    src-heftyad/model/view/graph/GraphViewEdge.h \
    src-heftyad/model/view/graph/GraphViewVertex.h \
    src-heftyad/model/view/View.h \
    src-heftyad/model/view/ViewItem.h \
    src-heftyad/model/view/ViewMultiShapeItem.h \
    src-heftyad/model/viewform/ArrayForm.h \
    src-heftyad/model/viewform/ViewForm.h \
    src-heftyad/plugin/form/AlgorithmPluginForm.h \
    src-heftyad/plugin/form/PluginForm.h \
    src-heftyad/plugin/AlgorithmPluginInterface.h \
    src-heftyad/plugin/PluginInterface.h \
    src-heftyad/scintilla/SciCodeEditor.h \
    src-heftyad/shadow/ShadowMessageBox.h \
    src-heftyad/simulation/console/SimulationShadowConsole.h \
    src-heftyad/simulation/console/SimulationSimpleConsole.h \
    src-heftyad/simulation/FakeAlgorithm.h \
    src-heftyad/simulation/SimulationAlgorithmFileViewer.h \
    src-heftyad/simulation/SimulationAlgorithmPluginPicker.h \
    src-heftyad/simulation/SimulationClarifier.h \
    src-heftyad/simulation/SimulationConfigWidget.h \
    src-heftyad/simulation/SimulationController.h \
    src-heftyad/simulation/SimulationHighlighter.h \
    src-heftyad/simulation/SimulationLocker.h \
    src-heftyad/simulation/SimulationSideWidget.h \
    src-heftyad/simulation/SimulationWidget.h \
    src-heftyad/simulation/SimulationWindow.h \
    src-heftyad/work/AlgorithmWork.h \
    src-heftyad/work/ModelWork.h \
    src-heftyad/work/Work.h \
    src-heftyad/work/WorkHelper.h \
    src-heftyad/workcontent/model/WorkArrayView.h \
    src-heftyad/workcontent/model/WorkGraphView.h \
    src-heftyad/workcontent/AlgorithmWorkContent.h \
    src-heftyad/workcontent/AlgorithmWorkFileViewer.h \
    src-heftyad/workspace/Workspace.h \
    src-heftyad/workspace/WorkspaceAlgorithmTab.h \
    src-heftyad/workspace/WorkspaceAlgorithmViewer.h \
    src-heftyad/workspace/WorkspaceBasicTab.h \
    src-heftyad/workspace/WorkspaceContextMenu.h \
    src-heftyad/workspace/WorkspaceEmptyTab.h \
    src-heftyad/workspace/WorkspaceModelTab.h \
    src-heftyad/workspace/WorkSpaceSimulationTab.h \
    src-heftyad/workspace/WorkspaceWorkContainer.h \
    src-heftyad/workspace/WorkspaceWorkEditor.h \
    src-heftyad/HomeWidget.h \
    src-heftyad/LanguagePicker.h \
    src-heftyad/MainData.h \
    src-heftyad/MainGuiData.h \
    src-heftyad/MainWindow.h \
    src-heftyad/Resource.h \
    src-heftyad/WelcomeTextAnimationWidget.h \
    src-heftyad/WelcomeWidget.h \
    src-libs/nicecpp/datastructure/Array.hpp \
    src-libs/nicecpp/datastructure/Automaton.hpp \
    src-libs/nicecpp/datastructure/ConnectionArea.hpp \
    src-libs/nicecpp/datastructure/Graph.hpp \
    src-libs/nicecpp/pattern/command/ShortHandUndoCommand.h \
    src-libs/nicecpp/pattern/command/UndoCommand.h \
    src-libs/nicecpp/pattern/command/UndoFactory.h \
    src-libs/nicecpp/pattern/command/UndoHelper.h \
    src-libs/nicecpp/pattern/command/UndoSequence.h \
    src-libs/nicecpp/pattern/command/UndoStack.h \
    src-libs/nicecpp/pattern/command/UndoStackGroup.h \
    src-libs/nicecpp/pattern/observer/Observable.h \
    src-libs/nicecpp/pattern/observer/Observer.h \
    src-libs/nicecpp/pattern/singleton/Singleton.hpp \
    src-libs/nicecpp/Nicecpplib.h \
    src-libs/nicecpp/nicecpplib_global.h \
    src-libs/qscintilla/Qsci/qsciabstractapis.h \
    src-libs/qscintilla/Qsci/qsciapis.h \
    src-libs/qscintilla/Qsci/qscicommand.h \
    src-libs/qscintilla/Qsci/qscicommandset.h \
    src-libs/qscintilla/Qsci/qscidocument.h \
    src-libs/qscintilla/Qsci/qsciglobal.h \
    src-libs/qscintilla/Qsci/qscilexer.h \
    src-libs/qscintilla/Qsci/qscilexeravs.h \
    src-libs/qscintilla/Qsci/qscilexerbash.h \
    src-libs/qscintilla/Qsci/qscilexerbatch.h \
    src-libs/qscintilla/Qsci/qscilexercmake.h \
    src-libs/qscintilla/Qsci/qscilexercoffeescript.h \
    src-libs/qscintilla/Qsci/qscilexercpp.h \
    src-libs/qscintilla/Qsci/qscilexercsharp.h \
    src-libs/qscintilla/Qsci/qscilexercss.h \
    src-libs/qscintilla/Qsci/qscilexercustom.h \
    src-libs/qscintilla/Qsci/qscilexerd.h \
    src-libs/qscintilla/Qsci/qscilexerdiff.h \
    src-libs/qscintilla/Qsci/qscilexerfortran.h \
    src-libs/qscintilla/Qsci/qscilexerfortran77.h \
    src-libs/qscintilla/Qsci/qscilexerhtml.h \
    src-libs/qscintilla/Qsci/qscilexeridl.h \
    src-libs/qscintilla/Qsci/qscilexerjava.h \
    src-libs/qscintilla/Qsci/qscilexerjavascript.h \
    src-libs/qscintilla/Qsci/qscilexerlua.h \
    src-libs/qscintilla/Qsci/qscilexermakefile.h \
    src-libs/qscintilla/Qsci/qscilexermatlab.h \
    src-libs/qscintilla/Qsci/qscilexeroctave.h \
    src-libs/qscintilla/Qsci/qscilexerpascal.h \
    src-libs/qscintilla/Qsci/qscilexerperl.h \
    src-libs/qscintilla/Qsci/qscilexerpo.h \
    src-libs/qscintilla/Qsci/qscilexerpostscript.h \
    src-libs/qscintilla/Qsci/qscilexerpov.h \
    src-libs/qscintilla/Qsci/qscilexerproperties.h \
    src-libs/qscintilla/Qsci/qscilexerpython.h \
    src-libs/qscintilla/Qsci/qscilexerruby.h \
    src-libs/qscintilla/Qsci/qscilexerspice.h \
    src-libs/qscintilla/Qsci/qscilexersql.h \
    src-libs/qscintilla/Qsci/qscilexertcl.h \
    src-libs/qscintilla/Qsci/qscilexertex.h \
    src-libs/qscintilla/Qsci/qscilexerverilog.h \
    src-libs/qscintilla/Qsci/qscilexervhdl.h \
    src-libs/qscintilla/Qsci/qscilexerxml.h \
    src-libs/qscintilla/Qsci/qscilexeryaml.h \
    src-libs/qscintilla/Qsci/qscimacro.h \
    src-libs/qscintilla/Qsci/qsciprinter.h \
    src-libs/qscintilla/Qsci/qsciscintilla.h \
    src-libs/qscintilla/Qsci/qsciscintillabase.h \
    src-libs/qscintilla/Qsci/qscistyle.h \
    src-libs/qscintilla/Qsci/qscistyledtext.h \
    src-niceqt/animation/TextAnimation.h \
    src-niceqt/dataserializer/DataXmlReader.h \
    src-niceqt/dataserializer/DataXmlWriter.h \
    src-niceqt/graphics/BasicGraphicsView.h \
    src-niceqt/graphics/GraphicsAlgo.h \
    src-niceqt/graphics/GraphicsEditableItem.h \
    src-niceqt/graphics/GraphicsItem.h \
    src-niceqt/graphics/GraphicsPixmapItem.h \
    src-niceqt/graphics/GraphicsTextItem.h \
    src-niceqt/graphics/GraphicsView.h \
    src-niceqt/graphics/GraphicsViewHighlighter.h \
    src-niceqt/graphics/GraphicsViewHighlighting.h \
    src-niceqt/graphics/HighlightableGraphicsView.h \
    src-niceqt/others/ActionGroup.h \
    src-niceqt/others/BasicLanguagePicker.h \
    src-niceqt/others/PluginLoader.h \
    src-niceqt/others/Thread.h \
    src-niceqt/others/Utility.h \
    src-niceqt/scripting/ScriptEngine.h \
    src-niceqt/widget/codeeditor/BasicXmlSyntaxHighlighter.h \
    src-niceqt/widget/codeeditor/SyntaxHighlighter.h \
    src-niceqt/widget/fileviewer/BasicFileSystemViewer.h \
    src-niceqt/widget/fileviewer/FileSystemViewer.h \
    src-niceqt/widget/fileviewer/ModelViewer.h \
    src-niceqt/widget/Dialog.h \
    src-niceqt/widget/ExtraToolBar.h \
    src-niceqt/widget/FormWidget.h \
    src-niceqt/widget/GridGroupBox.h \
    src-niceqt/widget/GridWidget.h \
    src-niceqt/widget/Menu.h \
    src-niceqt/widget/PushButton.h \
    src-niceqt/widget/TabWidget.h \
    src-niceqt/widget/TextAnimationWidget.h \
    src-niceqt/widget/TextEdit.h \
    src-niceqt/widget/TreedWidget.h \
    src-niceqt/winframe/BasicMainWindow.h \
    src-niceqt/widget/SectionedWidgetElt.h \
    src-niceqt/widget/SectionedWidgetEltBar.h \
    src-niceqt/widget/SectionedWidget.h \
    src-niceqt/widget/dataform/BrushFormWidget.h \
    src-niceqt/widget/dataform/ColorFormWidget.h \
    src-niceqt/widget/dataform/PixmapFormWidget.h \
    src-niceqt/widget/dataform/GradientFormWidget.h

SOURCES += \
    src-heftyad/algorithm/basis/Iterable.cpp \
    src-heftyad/algorithm/basis/SimplyControllable.cpp \
    src-heftyad/algorithm/Algorithm.cpp \
    src-heftyad/algorithm/AlgorithmRunner.cpp \
    src-heftyad/menu/EditMenu.cpp \
    src-heftyad/menu/FileMenu.cpp \
    src-heftyad/menu/SettingMenu.cpp \
    src-heftyad/model/animation/ModelPropertyAnimation.cpp \
    src-heftyad/model/inout/array/ArrayModelXmlReader.cpp \
    src-heftyad/model/inout/array/ArrayModelXmlWriter.cpp \
    src-heftyad/model/inout/graph/GraphModelXmlReader.cpp \
    src-heftyad/model/inout/graph/GraphModelXmlWriter.cpp \
    src-heftyad/model/inout/helpers/ModelReaderXmlHelper.cpp \
    src-heftyad/model/inout/helpers/ModelWriterXmlHelper.cpp \
    src-heftyad/model/inout/ModelReader.cpp \
    src-heftyad/model/inout/ModelWriter.cpp \
    src-heftyad/model/model/array/ArrayModel.cpp \
    src-heftyad/model/model/array/ArrayModelItem.cpp \
    src-heftyad/model/model/connectionarea/ModelCurve.cpp \
    src-heftyad/model/model/connectionarea/ModelNode.cpp \
    src-heftyad/model/model/graph/GraphModel.cpp \
    src-heftyad/model/model/graph/GraphModelEdge.cpp \
    src-heftyad/model/model/graph/GraphModelVertex.cpp \
    src-heftyad/model/model/Model.cpp \
    src-heftyad/model/model/ModelItem.cpp \
    src-heftyad/model/model/ModelMultiShapeItem.cpp \
    src-heftyad/model/view/array/ArrayView.cpp \
    src-heftyad/model/view/array/ArrayViewItem.cpp \
    src-heftyad/model/view/connectionarea/ViewCurve.cpp \
    src-heftyad/model/view/connectionarea/ViewCurvePoint.cpp \
    src-heftyad/model/view/connectionarea/ViewNode.cpp \
    src-heftyad/model/view/graph/GraphView.cpp \
    src-heftyad/model/view/graph/GraphViewEdge.cpp \
    src-heftyad/model/view/graph/GraphViewVertex.cpp \
    src-heftyad/model/view/View.cpp \
    src-heftyad/model/view/ViewItem.cpp \
    src-heftyad/model/view/ViewMultiShapeItem.cpp \
    src-heftyad/model/viewform/ArrayForm.cpp \
    src-heftyad/model/viewform/ViewForm.cpp \
    src-heftyad/plugin/form/AlgorithmPluginForm.cpp \
    src-heftyad/plugin/form/PluginForm.cpp \
    src-heftyad/plugin/AlgorithmPluginInterface.cpp \
    src-heftyad/plugin/PluginInterface.cpp \
    src-heftyad/scintilla/SciCodeEditor.cpp \
    src-heftyad/shadow/ShadowMessageBox.cpp \
    src-heftyad/simulation/console/SimulationShadowConsole.cpp \
    src-heftyad/simulation/console/SimulationSimpleConsole.cpp \
    src-heftyad/simulation/FakeAlgorithm.cpp \
    src-heftyad/simulation/SimulationAlgorithmFileViewer.cpp \
    src-heftyad/simulation/SimulationAlgorithmPluginPicker.cpp \
    src-heftyad/simulation/SimulationClarifier.cpp \
    src-heftyad/simulation/SimulationConfigWidget.cpp \
    src-heftyad/simulation/SimulationController.cpp \
    src-heftyad/simulation/SimulationHighlighter.cpp \
    src-heftyad/simulation/SimulationLocker.cpp \
    src-heftyad/simulation/SimulationSideWidget.cpp \
    src-heftyad/simulation/SimulationWidget.cpp \
    src-heftyad/simulation/SimulationWindow.cpp \
    src-heftyad/work/AlgorithmWork.cpp \
    src-heftyad/work/ModelWork.cpp \
    src-heftyad/work/Work.cpp \
    src-heftyad/work/WorkHelper.cpp \
    src-heftyad/workcontent/model/WorkArrayView.cpp \
    src-heftyad/workcontent/model/WorkGraphView.cpp \
    src-heftyad/workcontent/AlgorithmWorkContent.cpp \
    src-heftyad/workcontent/AlgorithmWorkFileViewer.cpp \
    src-heftyad/workspace/Workspace.cpp \
    src-heftyad/workspace/WorkspaceAlgorithmTab.cpp \
    src-heftyad/workspace/WorkspaceAlgorithmViewer.cpp \
    src-heftyad/workspace/WorkspaceBasicTab.cpp \
    src-heftyad/workspace/WorkspaceContextMenu.cpp \
    src-heftyad/workspace/WorkspaceEmptyTab.cpp \
    src-heftyad/workspace/WorkspaceModelTab.cpp \
    src-heftyad/workspace/WorkspaceSimulationTab.cpp \
    src-heftyad/workspace/WorkspaceWorkContainer.cpp \
    src-heftyad/workspace/WorkspaceWorkEditor.cpp \
    src-heftyad/HomeWidget.cpp \
    src-heftyad/LanguagePicker.cpp \
    src-heftyad/main.cpp \
    src-heftyad/MainData.cpp \
    src-heftyad/MainGuiData.cpp \
    src-heftyad/MainWindow.cpp \
    src-heftyad/Resource.cpp \
    src-heftyad/WelcomeTextAnimationWidget.cpp \
    src-heftyad/WelcomeWidget.cpp \
    src-niceqt/animation/TextAnimation.cpp \
    src-niceqt/dataserializer/DataXmlReader.cpp \
    src-niceqt/dataserializer/DataXmlWriter.cpp \
    src-niceqt/graphics/BasicGraphicsView.cpp \
    src-niceqt/graphics/GraphicsAlgo.cpp \
    src-niceqt/graphics/GraphicsEditableItem.cpp \
    src-niceqt/graphics/GraphicsItem.cpp \
    src-niceqt/graphics/GraphicsPixmapItem.cpp \
    src-niceqt/graphics/GraphicsTextItem.cpp \
    src-niceqt/graphics/GraphicsView.cpp \
    src-niceqt/graphics/GraphicsViewHighlighter.cpp \
    src-niceqt/graphics/GraphicsViewHighlighting.cpp \
    src-niceqt/graphics/HighlightableGraphicsView.cpp \
    src-niceqt/others/ActionGroup.cpp \
    src-niceqt/others/BasicLanguagePicker.cpp \
    src-niceqt/others/PluginLoader.cpp \
    src-niceqt/others/Thread.cpp \
    src-niceqt/others/Utility.cpp \
    src-niceqt/scripting/ScriptEngine.cpp \
    src-niceqt/widget/codeeditor/BasicXmlSyntaxHighlighter.cpp \
    src-niceqt/widget/codeeditor/SyntaxHighlighter.cpp \
    src-niceqt/widget/fileviewer/BasicFileSystemViewer.cpp \
    src-niceqt/widget/fileviewer/FileSystemViewer.cpp \
    src-niceqt/widget/fileviewer/ModelViewer.cpp \
    src-niceqt/widget/Dialog.cpp \
    src-niceqt/widget/ExtraToolBar.cpp \
    src-niceqt/widget/FormWidget.cpp \
    src-niceqt/widget/GridGroupBox.cpp \
    src-niceqt/widget/GridWidget.cpp \
    src-niceqt/widget/Menu.cpp \
    src-niceqt/widget/PushButton.cpp \
    src-niceqt/widget/TabWidget.cpp \
    src-niceqt/widget/TextAnimationWidget.cpp \
    src-niceqt/widget/TextEdit.cpp \
    src-niceqt/widget/TreedWidget.cpp \
    src-niceqt/winframe/BasicMainWindow.cpp \
    src-niceqt/widget/SectionedWidgetElt.cpp \
    src-niceqt/widget/SectionedWidgetEltBar.cpp \
    src-niceqt/widget/SectionedWidget.cpp \
    src-niceqt/widget/dataform/BrushFormWidget.cpp \
    src-niceqt/widget/dataform/ColorFormWidget.cpp \
    src-niceqt/widget/dataform/PixmapFormWidget.cpp \
    src-niceqt/widget/dataform/GradientFormWidget.cpp
    src-niceqt/widget/SectionedWidget.cpp

DISTFILES += \
    tools/copySharedLibs.bat \
