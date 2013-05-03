// Tabbed Pane project template
import bb.cascades 1.0

TabbedPane {
    id: tabbedPaneMain
    showTabsOnActionBar: true
    property variant selectedUserId
    property variant selectedFullName
    property variant selectedAction

    onCreationCompleted: {
        // this slot is called when declarative scene is created
        // write post creation initialization here
        console.log("TabbedPane - onCreationCompleted()")

        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
        sheetStartup.open();
    }
    Tab {
        id: tabStartup
        title: qsTr("Home")
        objectName: "tabStartup"
        NavigationPane
        {
            id: navigationPaneStartup
	        Page {
                actionBarVisibility: ChromeVisibility.Hidden
                Container {
	                id: containerStartup
	                topPadding: 200.0
	                horizontalAlignment: HorizontalAlignment.Center
	                Label {
	                    text: "Open Peer BB10 SDK"
	                    textFormat: TextFormat.Plain
	                    textStyle.base: SystemDefaults.TextStyles.BigText
	                    textStyle.fontSize: FontSize.Default
	                    textStyle.textAlign: TextAlign.Center
	                    textStyle.color: Color.Gray
	                    horizontalAlignment: HorizontalAlignment.Center
                    }
	                Label {
	                    text: "Sample App"
	                    textFormat: TextFormat.Plain
	                    textStyle.base: SystemDefaults.TextStyles.BigText
	                    textStyle.fontSize: FontSize.Default
	                    textStyle.textAlign: TextAlign.Center
	                    textStyle.color: Color.Gray
	                    horizontalAlignment: HorizontalAlignment.Center
	                }
	                Button {
	                    text: "Login through FaceBook"
	                    onClicked: {
	                        console.log("*** sheetWebview: closing");
                            var page = pageWebviewCompDef.createObject();
                            navigationPaneStartup.push(page);
	                    }
	                    topMargin: 150.0
	                    horizontalAlignment: HorizontalAlignment.Center
	
	                }
	            }
	        }
        }
        attachedObjects: [
            ComponentDefinition {
                id: pageWebviewCompDef
                Page {
                    id: pageWebview
                    Container {
                        id: containerWebview
                        Label {
                            id: labelLoading
                            text: "Loading"
                            textFormat: TextFormat.Plain
                            textStyle.base: SystemDefaults.TextStyles.BigText
                            textStyle.fontSize: FontSize.Default
                            textStyle.textAlign: TextAlign.Center
                            textStyle.color: Color.Gray
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                        ActivityIndicator {
                            id: activityIndicatorWebview
                            running: true
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            scaleX: 1.0
                            scaleY: 1.0
                            preferredWidth: 100.0
                            preferredHeight: 100.0
                            topMargin: 0.0
                            topPadding: 0.0
                            translationX: 0.0
                            translationY: 440.0
                        }

                        Button {
                            id: buttonShowWebview
                            text: "Show Webview"
                            onClicked: {
                                console.log("*** sheetWebview: showing webview");
                                activityIndicatorWebview.visible = false;
                                labelLoading.visible = false;
                                buttonShowWebview.visible = false;
                                webviewLogin.visible = true;
                            }
                        }
                        WebView {
                            id: webviewLogin
                            url: "http://google.com"
                            visible: false

                        }
                    }
                    actions: [
                        ActionItem {
                            title: "Debug Success"
                            ActionBar.placement: ActionBarPlacement.OnBar

                            onTriggered: {

                                tabbedPaneMain.activePane = pageContacts;
                                tabbedPaneMain.remove(tabStartup);
                            }
                        }
                    ] // actions
                } // Page
            } // ComponentDefinition
        ]

    }
    Tab {
        id: tabContacts
        title: qsTr("Contacts")
        objectName: "tabContacts"
        imageSource: "asset:///images/contacts.png"
        NavigationPane {
            id: navigationPaneContacts
            Page {
	            id: pageContacts
	            actions: [
	                // define the actions for tab here
	                ActionItem {
	                    title: qsTr("Raise")
	                    onTriggered: {
	                        // run the image animation
	                        raiseAnimation.play();
	                    }
	                }
	            ]
	            objectName: "tabContactsPage"
	            Container {
	                id: containerContacts
                    ListView {
                        id: listViewContacts
                        dataModel: XmlDataModel {
                            source: "asset:///models/contactModel.xml"
                        }
                        listItemComponents: [
                            ListItemComponent {
                                type: "contactitem"
                                Container {
                                    leftPadding: 10.0
                                    horizontalAlignment: HorizontalAlignment.Fill

                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    topPadding: 10.0
                                    bottomPadding: 1.0
                                    rightPadding: 10.0
                                    ImageView {
                                        imageSource: ListItemData.pictureUrl
                                        preferredWidth: 100.0
                                        preferredHeight: 100.0
                                        verticalAlignment: VerticalAlignment.Center
                                    }
                                    Label {
                                        text: ListItemData.fullName
                                        verticalAlignment: VerticalAlignment.Center
                                        textStyle.fontStyle: FontStyle.Default
                                        textStyle.fontWeight: FontWeight.Default
                                        textStyle.base: SystemDefaults.TextStyles.BigText
                                        textStyle.fontSize: FontSize.XLarge
                                    }
                                }
                            }
                        ]
                        onTriggered: {
                            // When an item is selected, we push the contact action Page in the chosenItem file attribute.
                            var chosenItem = dataModel.data(indexPath);
                            tabbedPaneMain.selectedUserId = chosenItem.id;
                            tabbedPaneMain.selectedFullName = chosenItem.fullName;

                            var page = pageContactActionCompDef.createObject();
                            navigationPaneContacts.push(page);
                        }

                    }
	            }
            } // Page
        } // Navigation Pane

        attachedObjects: [
            ComponentDefinition {
                id: pageContactActionCompDef
                Page {
                    id: contactAction
                    onCreationCompleted: {
                        labelContactName.text = tabbedPaneMain.selectedFullName;
                    }
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
                            textStyle.fontSize: FontSize.Medium
                            bottomMargin: 0.0
                        }
                        Button {
                            id: buttonVideoCall
                            text: "Video Call"
                            horizontalAlignment: HorizontalAlignment.Center
                            topPadding: 0.0
                            topMargin: 50.0
                            onClicked: {
                                navigationPaneContacts.pop();
                                tabbedPaneMain.activePane = pageVideo;
                                tabbedPaneMain.activeTab = tabVideo;
                            }
                        }
                        Button {
                            id: buttonVoiceCall
                            text: "Voice Call"
                            horizontalAlignment: HorizontalAlignment.Center
                            onClicked: {
                                navigationPaneContacts.pop();
                                tabbedPaneMain.activePane = pageVideo;
                                tabbedPaneMain.activeTab = tabVideo;
                            }
                        }
                        Button {
                            id: buttonText
                            text: "Send Text Message"
                            horizontalAlignment: HorizontalAlignment.Center
                            onClicked: {
                                navigationPaneContacts.pop();
                                tabbedPaneMain.activePane = pageText;
                                tabbedPaneMain.activeTab = tabText;
                            }
                        }
                    }
                } // Page
            } // ComponentDefinition
        ]

    }
    Tab {
        id: tabText
        title: qsTr("Text")
        objectName: "tabText"
        enabled: true
        imageSource: "asset:///images/chat.png"
        Page {
            id: pageText
            objectName: "pageText"
            actionBarVisibility: ChromeVisibility.Visible
            Container {
                Container {
                    background: Color.Black
                    preferredHeight: maxHeight
                    //! [1]
                    TextArea {
                        textStyle {
                            base: SystemDefaults.TextStyles.SubtitleText
                            fontWeight: FontWeight.Bold
                            color: Color.Yellow
                        }

                        backgroundVisible: true
                        editable: false
                        text: _btController.chatManager.chatHistory
                    }
                    //! [1]
                } // Container

                Container {
                    leftPadding: 10
                    rightPadding: 10
                    bottomPadding: 10
                    topPadding: 10

                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }

                    //! [2]
                    TextField {
                        id: textInput

                        layoutProperties: StackLayoutProperties {
                            spaceQuota: .95
                        }

                        hintText: qsTr("Type a message to send")

                        inputMode: TextAreaInputMode.Chat
                        input {
                            submitKey: SubmitKey.Send
                        }

                        onTextChanged: {
                            if (text.length > 0) {
                                _btController.chatManager.sendSPPMessage(text);
                                textInput.text = ""
                            }
                        }
                    }
                } // Container
            }
        }
    }
    Tab {
        id: tabVideo
        title: qsTr("Call")
        onTriggered: {
            paneParent.OnCallWindowOpened(callTab);
        }
        imageSource: "asset:///images/videocall.png"
        Page {
            id: pageVideo
            onCreationCompleted: {
            }
            Container {
                ForeignWindowControl {
                    id: foreignWindow
                    objectName: "foreignWindow"
                    visible: true
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                }
            }
            titleBar: TitleBar {
                id: addBar
                title: "Calling..."
                visibility: ChromeVisibility.Visible

                dismissAction: ActionItem {
                    title: "End"
                    onTriggered: {

                    }
                }

                acceptAction: ActionItem {
                    title: "Mute"
                    onTriggered: {
                    }
                }
            }
        }
    }
}