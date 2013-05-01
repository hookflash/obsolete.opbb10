// Tabbed Pane project template
import bb.cascades 1.0
import "Common"

TabbedPane {
    id: pageMain
    showTabsOnActionBar: true
    Tab {
        title: qsTr("Connect")
        imageSource: "asset:///images/icon_home.png"
        NavigationPane {
            id: navPane1
	        Page {
	            id: tab1
	            actions: [
	                // define the actions for first tab here
	                ActionItem {
	                    title: qsTr("Rotate")
	                    onTriggered: {
	                        imgTab1.rotationZ = imgTab1.rotationZ + 90;
	                    }
	                },
	                ActionItem {
	                    title: qsTr("Break")
	                    onTriggered: {
	                        imgTab1.imageSource = "asset:///images/picture1br.png";
	                    }
	                }
	            ]
	            Container {
	                background: Color.Black
	                topMargin: 200.0
	                topPadding: 200.0
	
	                // define tab content here
	
	                ImageView {
	                    id: imgTab1
	                    imageSource: "asset:///images/logo.png"
	                    layoutProperties: StackLayoutProperties {
	                        // make imageView to fill all available height
	                        spaceQuota: 1.0
	                    }
	                    verticalAlignment: VerticalAlignment.Center
	                    horizontalAlignment: HorizontalAlignment.Center
	                    scalingMethod: ScalingMethod.AspectFit
	                    enabled: true
	                    opacity: 1.0
	                    minWidth: 208.0
	                    minHeight: 50.0
	                    preferredWidth: 208.0
	                    preferredHeight: 50.0
	                    maxWidth: 208.0
	                    maxHeight: 50.0
	                    topMargin: 200.0
	                    topPadding: 200.0
                    }
                    Button {
	                    id: btnLoginViaFB
	                    text: "Login through Facebook"
	                    horizontalAlignment: HorizontalAlignment.Center
	                    topMargin: 200.0
	                    topPadding: 400.0
	                    onClicked: {
	                        paneParent.OnLoginClick(navPane1);
                        }
	                    attachedObjects: [
	                        ComponentDefinition {
	                            id: weblogin
	                            source: "weblogin.qml"
	
	                        }
	
	                    ]
	
	                }
                 }
            }
        }
    }

    Tab {
        title: qsTr("Contacts")
        enabled: true
        imageSource: "asset:///images/contacts.png"
        NavigationPane {
            id: contactsNavPane
            onCreationCompleted: {
                console.log("**** onCreationCompleted of...");
            }
            objectName: "contactsNavPane"
            Page {
	            id: contactsPage
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
	            Container {
	                // define tab content here
	                ListView {
	                    id: contactList
                        objectName: "contactList"
                        dataModel: GroupDataModel {
                        }

                        listItemComponents: [
	                        ListItemComponent {
	                            type: "contactItem"
	                            ContactItem {
	                            }
	                        }
	                    ]
	                    onTriggered: {
	                        // When an item is selected, we push the contact action Page in the chosenItem file attribute.
	                        console.log("*** contactList.onTriggered");
	                        var chosenItem = dataModel.data(indexPath);
	                        console.log("*** chosenItem.userId = " + chosenItem.userid);
	
	                        // Set the correct file source on the ComponentDefinition, create the Page, and set its title.
	//                        contactAction.id = chosenItem.userid;
	                        var page = contactActionPage.createObject();
	
	                        // Push the new Page.
	                        contactsNavPane.push(page);
	                    }
	
	                } // ListView
	            } // Container
            } // NavigationPane
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: contactActionPage
            source: "ContactAction.qml"
        }
    ]
    Tab {
        id: videoCallTab
        title: qsTr("Call")
        onTriggered: {
            paneParent.OnCallWindowOpened(callTab);
        }
        imageSource: "asset:///images/videocall.png"
        Page {
            id: callTab
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
/*
                Container {
                    layout: AbsoluteLayout {

                    }
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    }
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    Button {
                        id: btnMediaTest1
                        text: "1"
                        onClicked: {
                            paneParent.OnMediaTestButton1Click();
                        }
                        layoutProperties: AbsoluteLayoutProperties {
                            positionX: 20.0
                            positionY: 1000.0

                        }
                    }
                    Button {
                        id: btnMediaTest2
                        text: "2"
                        onClicked: {
                            paneParent.OnMediaTestButton2Click();
                        }
                        layoutProperties: AbsoluteLayoutProperties {
                            positionX: 390.0
                            positionY: 1000.0

                        }
                    }
                }
*/
            }
        }
    }
    Tab {
        title: qsTr("Settings")

        Page {
            id: tab3
            Container {
                // define tab content here
                Label {
                    text: qsTr("Tab 3 title")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
                Container {
                    layout: DockLayout {
                    }
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    }
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    Label {
                        text: qsTr("Tab 3 content")
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                        }
                    }
                }
            }
        }
    }
    onCreationCompleted: {
        // this slot is called when declarative scene is created
        // write post creation initialization here
        console.log("TabbedPane - onCreationCompleted()")

        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }
}
