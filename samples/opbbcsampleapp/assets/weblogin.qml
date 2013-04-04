import bb.cascades 1.0

Page {
    id: pageWebLogin

    Container {
        touchPropagationMode: TouchPropagationMode.Full

        WebView {
        	id: webView
            url: "http://hookflash.me"
            onNavigationRequested: {
            }
            onLoadingChanged: {
                parentPane.OnLoadingChanged(loadRequest.status, loadRequest.url);

                console.log("*** onLoadingChanged("+loadRequest.status+" url="+ loadRequest.url);
            }
        }
        ActivityIndicator {

        }

    }
}
