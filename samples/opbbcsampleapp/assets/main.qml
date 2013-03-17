// Tabbed Pane project template
import bb.cascades 1.0
    
TabbedPane {
    id: pageMain
    showTabsOnActionBar: true
    Tab {
        title: qsTr("Connect")
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
	                        var loginPage = weblogin.createObject();
	                        navPane1.push(loginPage);
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
        Page {
            id: tab2
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
                Label {
                    text: qsTr("Tab 2 title")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
                ImageView {
                    id: imgTab2
                    imageSource: "asset:///images/picture1.png"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    }
                    scalingMethod: ScalingMethod.AspectFit
                    opacity: 0.2
                    animations: [
                        // define animations for image here
                        ParallelAnimation {
                            id: raiseAnimation
                            FadeTransition {
                                fromOpacity: 0.2
                                toOpacity: 1
                                duration: 1000
                            }
                            ScaleTransition {
                                fromX: 1
                                fromY: 1
                                toX: 1.5
                                toY: 1.5
                                duration: 1000
                                easingCurve: StockCurve.DoubleElasticOut
                            }
                        }
                    ]
                }
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
                    layout: DockLayout { }
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    }
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    Label {
                        text: qsTr ("Tab 3 content")
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
