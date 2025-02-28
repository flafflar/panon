import QtQuick 2.0
import QtQuick.Layouts 1.1

import org.kde.plasma.plasmoid
import org.kde.plasma.core as PlasmaCore

import QtQuick.Controls 2.0 as QQC2

import "utils.js" as Utils

Item{
    id:root
    readonly property var cfg:plasmoid.configuration

    property bool vertical: (plasmoid.formFactor == PlasmaCore.Types.Vertical)

    // Layout.minimumWidth:  cfg.autoHide ? animatedMinimum: -1
    Layout.preferredWidth: vertical ?-1: animatedMinimum
    Layout.preferredHeight: vertical ?  animatedMinimum:-1
    Layout.maximumWidth:cfg.autoHide?Layout.preferredWidth:-1
    Layout.maximumHeight:cfg.autoHide?Layout.preferredHeight:-1

    // Gravity property: Center(0), North (1), West (4), East (3), South (2)
    readonly property int gravity:{
        if(cfg.gravity>0)
            return cfg.gravity
        switch(plasmoid.location){
            case PlasmaCore.Types.TopEdge:
            return 2
            case PlasmaCore.Types.BottomEdge:
            return 1
            case PlasmaCore.Types.RightEdge:
            return 3
            case PlasmaCore.Types.LeftEdge:
            return 4
        }
        return 1
    }

    property int animatedMinimum:(!cfg.autoHide) || audioAvailable? cfg.preferredWidth:0

    Layout.fillWidth: vertical? false:cfg.autoExtend
    Layout.fillHeight: vertical? cfg.autoExtend :false


    ShaderEffect {
        id:mainSE
        readonly property bool colorSpaceHSL: cfg.colorSpaceHSL
        readonly property bool colorSpaceHSLuv:cfg.colorSpaceHSLuv

        Behavior on hueFrom{ NumberAnimation { duration: 1000} }
        Behavior on hueTo{ NumberAnimation { duration: 1000} }
        Behavior on saturation{ NumberAnimation { duration: 1000} }
        Behavior on lightness{ NumberAnimation { duration: 1000} }

        property int hueFrom    :{
            if(cfg.colorSpaceHSL)
                return cfg.hslHueFrom
            else if(cfg.colorSpaceHSLuv)
                return cfg.hsluvHueFrom
        }
        property int hueTo    :{
            if(cfg.colorSpaceHSL)
                return cfg.hslHueTo
            else if(cfg.colorSpaceHSLuv)
                return cfg.hsluvHueTo
        }
        property int saturation  :{
            if(cfg.colorSpaceHSL)
                return cfg.hslSaturation
            else if(cfg.colorSpaceHSLuv)
                return cfg.hsluvSaturation
        }
        property int lightness   :{
            if(cfg.colorSpaceHSL)
                return cfg.hslLightness
            else if(cfg.colorSpaceHSLuv)
                return cfg.hsluvLightness
        }

        readonly property variant iMouse:iMouseArea.i

        property double iTime
        property double iTimeDelta
        property double iBeat
        property variant iResolution:root.gravity<=2?Qt.vector3d(mainSE.width,mainSE.height,0):Qt.vector3d(mainSE.height,mainSE.width,0)
        property int iFrame:0
        property vector3d iChannelResolution0:iChannel0?Qt.vector3d(iChannel0.width,iChannel0.height,0):Qt.vector3d(0,0,0)
        property vector3d iChannelResolution1:iChannel1?Qt.vector3d(iChannel1.width,iChannel1.height,0):Qt.vector3d(0,0,0)
        property vector3d iChannelResolution2:iChannel2?Qt.vector3d(iChannel2.width,iChannel2.height,0):Qt.vector3d(0,0,0)
        property vector3d iChannelResolution3:iChannel3?Qt.vector3d(iChannel3.width,iChannel3.height,0):Qt.vector3d(0,0,0)
        property variant iChannel0
        property variant iChannel1
        readonly property variant iChannel2:bufferSES
        readonly property variant iChannel3:Image{source:'file://'+shaderSourceReader.texture_uri}

        property int coord_gravity:root.gravity
        property bool coord_inversion:cfg.inversion

        property int iParam0:root.cfg_effectArgValues?.[0] ?? 0
        property int iParam1:root.cfg_effectArgValues?.[1] ?? 0
        property int iParam2:root.cfg_effectArgValues?.[2] ?? 0
        property int iParam3:root.cfg_effectArgValues?.[3] ?? 0
        property int iParam4:root.cfg_effectArgValues?.[4] ?? 0
        property int iParam5:root.cfg_effectArgValues?.[5] ?? 0
        property int iParam6:root.cfg_effectArgValues?.[6] ?? 0
        property int iParam7:root.cfg_effectArgValues?.[7] ?? 0
        property int iParam8:root.cfg_effectArgValues?.[8] ?? 0
        property int iParam9:root.cfg_effectArgValues?.[9] ?? 0
        property double fParam0:root.cfg_effectArgValues?.[0] ?? 0
        property double fParam1:root.cfg_effectArgValues?.[1] ?? 0
        property double fParam2:root.cfg_effectArgValues?.[2] ?? 0
        property double fParam3:root.cfg_effectArgValues?.[3] ?? 0
        property double fParam4:root.cfg_effectArgValues?.[4] ?? 0
        property double fParam5:root.cfg_effectArgValues?.[5] ?? 0
        property double fParam6:root.cfg_effectArgValues?.[6] ?? 0
        property double fParam7:root.cfg_effectArgValues?.[7] ?? 0
        property double fParam8:root.cfg_effectArgValues?.[8] ?? 0
        property double fParam9:root.cfg_effectArgValues?.[9] ?? 0

        anchors.fill: parent
        blending: true
        fragmentShader:shaderSourceReader.image_shader_source
    }

    ShaderEffectSource {
        visible:false
        id:bufferSES
        width: mainSE.iResolution.x
        height: mainSE.iResolution.y
        recursive :true
        live:false
        sourceItem: ShaderEffect {
            width: mainSE.iResolution.x
            height: mainSE.iResolution.y
            readonly property bool colorSpaceHSL: mainSE.colorSpaceHSL
            readonly property bool colorSpaceHSLuv:mainSE.colorSpaceHSLuv
            readonly property int hueFrom:mainSE.hueFrom
            readonly property int hueTo:mainSE.hueTo
            readonly property int saturation:mainSE.saturation
            readonly property int lightness:mainSE.lightness

            readonly property double iTime:mainSE.iTime
            readonly property double iTimeDelta:mainSE.iTimeDelta
            readonly property double iBeat:mainSE.iBeat
            readonly property variant iResolution:mainSE.iResolution
            readonly property int iFrame:mainSE.iFrame
            readonly property vector3d iChannelResolution0:mainSE.iChannelResolution0
            readonly property vector3d iChannelResolution1:mainSE.iChannelResolution1
            readonly property vector3d iChannelResolution2:mainSE.iChannelResolution2
            readonly property vector3d iChannelResolution3:mainSE.iChannelResolution3
            readonly property variant iChannel0:mainSE.iChannel0
            readonly property variant iChannel1:mainSE.iChannel1
            readonly property variant iChannel2:mainSE.iChannel2
            readonly property variant iChannel3:mainSE.iChannel3
            readonly property variant iMouse:mainSE.iMouse

            readonly property int coord_gravity:mainSE.coord_gravity
            readonly property bool coord_inversion:mainSE.coord_inversion

            property int iParam0:mainSE.iParam0
            property int iParam1:mainSE.iParam1
            property int iParam2:mainSE.iParam2
            property int iParam3:mainSE.iParam3
            property int iParam4:mainSE.iParam4
            property int iParam5:mainSE.iParam5
            property int iParam6:mainSE.iParam6
            property int iParam7:mainSE.iParam7
            property int iParam8:mainSE.iParam8
            property int iParam9:mainSE.iParam9
            property int fParam0:mainSE.fParam0
            property int fParam1:mainSE.fParam1
            property int fParam2:mainSE.fParam2
            property int fParam3:mainSE.fParam3
            property int fParam4:mainSE.fParam4
            property int fParam5:mainSE.fParam5
            property int fParam6:mainSE.fParam6
            property int fParam7:mainSE.fParam7
            property int fParam8:mainSE.fParam8
            property int fParam9:mainSE.fParam9

            fragmentShader:shaderSourceReader.buffer_shader_source
        }
    }

    /*
    ShaderEffectSource {
        id:glDFTSES
        width: glDFTSE.width
        height: glDFTSE.height
        visible:false
        live:false
        sourceItem: ShaderEffect {

            id:glDFTSE
            width: 200
            height: 1
            property int dftSize:glDFTSE.width
            property int bufferSize:waveBufferSE.width
            fragmentShader:shaderSourceReader.gldft_source

            readonly property variant waveBuffer:ShaderEffectSource {
                id:waveBufferSES
                width: waveBufferSE.width
                height: waveBufferSE.height
                live:false
                sourceItem: ShaderEffect {
                    id:waveBufferSE
                    width: 2000
                    height: 2
                    property variant newWave
                    property int bufferSize:waveBufferSE.width
                    property int newWaveSize:newWave?newWave.width:0
                    readonly property variant waveBuffer:waveBufferSES
                    fragmentShader:shaderSourceReader.wave_buffer_source
                }
            }
        }
    }
    */

    readonly property bool loadImageShaderSource:   shaderSourceReader.image_shader_source.trim().length>0
    readonly property bool loadBufferShaderSource:  shaderSourceReader.buffer_shader_source.trim().length>0
    readonly property bool failCompileImageShader:  loadImageShaderSource && false // (mainSE.status==ShaderEffect.Error)
    readonly property bool failCompileBufferShader: loadBufferShaderSource && false // (bufferSES.sourceItem.status==ShaderEffect.Error)
    property string fps_message:""
    property string error_message:
        shaderSourceReader.error_message
        + (loadImageShaderSource ?"":i18n("Error: Failed to load the visual effect. Please choose another visual effect in the configuration dialog."))
        + (failCompileImageShader?(i18n("Error: Failed to compile image shader.")+mainSE.log):"")
        + (failCompileBufferShader?(i18n("Error: Failed to compile bufffer shader.")+bufferSES.sourceItem.log):"")
    QQC2.Label {
        id:console_output
        anchors.fill: parent
        color: PlasmaCore.ColorScope.textColor
        text:error_message+(cfg.showFps?fps_message:"")
    }

    MouseArea {
        id:iMouseArea
        hoverEnabled :true
        anchors.fill: parent

        readonly property double current_x:root.gravity<3?(cfg.inversion?(mainSE.width- mouseX):mouseX):(cfg.inversion?mouseY:(mainSE.height-mouseY))
        readonly property double current_y:[mainSE.height- mouseY,mouseY ,mainSE.width-mouseX ,mouseX ][root.gravity-1]
        property double lastdown_x
        property double lastdown_y
        property double lastclick_x
        property double lastclick_y

        property var i:Qt.vector4d(lastdown_x,lastdown_y ,pressed?lastclick_x:-lastclick_x,-lastclick_y)
        onPressed:{
            lastclick_x=current_x
            lastclick_y=current_y

            lastdown_x=current_x
            lastdown_y=current_y
        }
        onPositionChanged:{
            if(pressed){
                lastdown_x=current_x
                lastdown_y=current_y
            }
        }
    }

    ShaderSource{id:shaderSourceReader}

    WsConnection{
        shaderSourceReader:shaderSourceReader
        queue:MessageQueue{
            only_spectrum:shaderSourceReader.enable_iChannel1 && !shaderSourceReader.enable_iChannel0
            onImgsReadyChanged:{

                audioAvailable=imgsReady.audioAvailable
                var time_current_frame=Date.now()
                var deltatime=(time_current_frame-time_prev_frame)/1000.0
                mainSE.iTime=(time_current_frame-time_first_frame) /1000.0
                mainSE.iTimeDelta=deltatime
                mainSE.iFrame+=1
                if(cfg.showFps)
                    if(mainSE.iFrame%30==1){
                        fps_message='fps:'+ Math.round(1000*30/(time_current_frame-time_fps_start))
                        time_fps_start=time_current_frame
                    }

                        
                if(cfg.glDFT){
                    /*
                    waveBufferSE.newWave=imgsReady.w;
                    waveBufferSES.scheduleUpdate();
                    glDFTSES.scheduleUpdate();
                    mainSE.iChannel1=glDFTSES;
                    */
                }else{
                    mainSE.iChannel0=imgsReady.w;
                    mainSE.iChannel1=imgsReady.s;
                    mainSE.iBeat=imgsReady.beat;
                }
                if(shaderSourceReader.enable_buffer)
                    bufferSES.scheduleUpdate();

                time_prev_frame=time_current_frame

            }
        }
    }

    property bool audioAvailable

    property double time_first_frame:Date.now()
    property double time_fps_start:Date.now()
    property double time_prev_frame:Date.now()
    Behavior on animatedMinimum{
        enabled:cfg.animateAutoHiding
        NumberAnimation {
            duration: 250
            easing.type: Easing.InCubic
        }
    }
}

