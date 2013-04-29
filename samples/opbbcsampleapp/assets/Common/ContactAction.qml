import bb.cascades 1.0

Page {
    id: contactAction

    Container {
        horizontalAlignment: HorizontalAlignment.Center
        topMargin: 0.0
        topPadding: 300.0
        Label {
            id: labelContactName
            objectName: "labelContactName"
            text: "Contact Email"
            horizontalAlignment: HorizontalAlignment.Center
            textFormat: TextFormat.Plain
            textStyle.textAlign: TextAlign.Center
            topMargin: 0.0
            topPadding: 0.0

        }
        Button {
            id: buttonVideoCall
            text: "Video Call"
            horizontalAlignment: HorizontalAlignment.Center
            topPadding: 0.0
            topMargin: 50.0
            onClicked: {
                contactsNavPane.pop();

            }

        }
        Button {
            id: buttonVoiceCall
            text: "Voice Call"
            horizontalAlignment: HorizontalAlignment.Center

        }
        Button {
            id: buttonText
            text: "Send Text Message"
            horizontalAlignment: HorizontalAlignment.Center

        }
    }
}
