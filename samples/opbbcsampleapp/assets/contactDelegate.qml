import QtQuick 1.0
 
Item {
  id: delegate
  height: 30
  width: delegate.ListView.view.width
 
  Text {
    anchors.fill: parent;
    text: name +" ("+size+")"
  }
}