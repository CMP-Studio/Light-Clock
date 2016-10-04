import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: '../../..'

    ofApp {
        name: { return FileInfo.baseName(path) }

        files: [
            'src/main.cpp',
            'src/ofApp.cpp',
            'src/ofApp.h',
            'src/dayFade.cpp',
            'src/dayFade.h',
            'src/oneImage.cpp',
            'src/oneImage.h',
            'src/fileManager.cpp',
            'src/fileManager.h',
            'src/rot/rotationSensing.h',
            'src/rot/rotationSensing.cpp',
            'src/flk/Boid_mod.cpp',
            'src/flk/Boid_mod.h',
            'src/flk/Flocking.cpp',
            'src/flk/Flocking.h',
            'src/flk/Obstacle.cpp',
            'src/flk/Obstacle.h',
            'src/flk/singleBoid.h',
            'src/flk/singleBoid.cpp',
        ]

        // This project is using addons.make to include the addons
        // since it was imported from old code. To change it to include
        // the addons from the qbs file change the following lines to
        // the list of used addons in array format. eg:
        //
         of.addons: [
             'ofxGui',
             'ofxOpenCv',
             'ofxEasing',
             'ofxCameraSaveLoad',
             'ofxOpticalFlowFarneback',
             'ofxV4L2Settings',
             'ofxCsv',
         ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: ['libv4l2']       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
        // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
