import bb.cascades 1.0

Page {
	id: videoCall
    Menu.definition: MenuDefinition {

        // Specify the actions that should be included in the menu
        actions: [
            ActionItem {
                title: "Action 1"
                imageSource: "images/actionOneIcon.png"

                onTriggered: {
                    textField.text = "Action 1 selected!"
                }
            },
            ActionItem {
                title: "Action 2"
                imageSource: "images/actionTwoIcon.png"

                onTriggered: {
                    textField.text = "Action 2 selected!"
                }
            },
            ActionItem {
                title: "Action 3"

                onTriggered: {
                    textField.text = "Action 3 selected!"
                }
            }
        ] // end of actions list
    } // end of MenuDefinition
    Container {
        Label {
            text: "Hi"
            textStyle.textAlign: TextAlign.Center
            textStyle.fontWeight: FontWeight.Default
            textFormat: TextFormat.Plain
            horizontalAlignment: HorizontalAlignment.Center

        }
    }
}
