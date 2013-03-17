import bb.cascades 1.0

Page {
    id: pageWebLogin

    Container {
        touchPropagationMode: TouchPropagationMode.Full

        WebView {
            url: "http://google.com"
            onNavigationRequested: {
            }
            onLoadingChanged: {
                console.log("onLoadingChanged: status=" + loadRequest.status);
                if (loadRequest.status == WebLoadStatus.Started) console.log("Loading started...");
                if (loadRequest.status == WebLoadStatus.Failed) {
                    console.log("Page load failed!");
                }
                if (loadRequest.status == WebLoadStatus.Succeeded) {
                    console.log("Page loaded!");
                }
            }
        }
        ActivityIndicator {

        }

    }
}
