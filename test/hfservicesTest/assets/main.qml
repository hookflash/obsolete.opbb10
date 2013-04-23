// Default empty project template
import bb.cascades 1.0

// creates one page with a label
Page {
    Container {
        layout: DockLayout {}
        Label {
            text: qsTr("hfservices Test")
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
    }
}

