import bb.cascades 1.0

Page {
    id: pageWebLogin

    Container {
        touchPropagationMode: TouchPropagationMode.Full
        objectName: "containerObj"
        WebView {
        	id: webView
            url: "http://hookflash.me"
            onNavigationRequested: {
            }
            onLoadingChanged: {
            	paneParent.OnLoadingChanged(loadRequest.status, loadRequest.url);

                console.log("*** onLoadingChanged("+loadRequest.status+" url="+ loadRequest.url);
            }

            function setLoginPane(cppParent) {
              cppParent.TestCallback();
            }
            objectName: "webViewObj"
            touchPropagationMode: TouchPropagationMode.Full
        }
        ActivityIndicator {

        }

    }
}
