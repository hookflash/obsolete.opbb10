import bb.cascades 1.0

Page {
    id: pageWebLogin

    Container {
        touchPropagationMode: TouchPropagationMode.Full
        id: "containerObj"
        objectName: "containerObj"
        ScrollView {
            scrollViewProperties.scrollMode: ScrollMode.Both
            scrollViewProperties.maxContentScale: 2.0
            WebView {
	        	id: "webView"
	        	objectName: "webView"
	            url: "https://app-light.hookflash.me/outer.html"
	            settings.customHttpHeaders: {
	                "Pragma": "no-cache"
	            }
	            onNavigationRequested: {
	                var cancel = paneParent.OnNavigationRequested(request.url);
	
	                console.log("*** onNavigationRequested(" + request.navigationType + " url=" + request.url + " cancel = " + cancel);
	
	                if(cancel) {
	                    request.action = WebNavigationRequestAction.Ignore;
	                }
	                else {
	                	request.action = WebNavigationRequestAction.Accept;
	                }
	            }
	            onLoadingChanged: {
	                console.log("*** onLoadingChanged(" + loadRequest.status + " url=" + loadRequest.url);
	                paneParent.OnLoadingChanged(loadRequest.status, loadRequest.url);
	            }
	
	            function setLoginPane(cppParent) {
	                cppParent.TestCallback();
	            }
	            function callPageJavaScriptFunction(functionName, args) {
	            	value = functionName+'("'+args+'")';
	            	console.log("*** "+value);
	                return webView.evaluateJavaScript(value);
	            }
	            touchPropagationMode: TouchPropagationMode.Full
	            visible: true
	            verticalAlignment: VerticalAlignment.Top
	            onLoadProgressChanged: {
	            }
	        } // WebView
        } // ScrollView
        Label {
            id: loadingLabel
            objectName: "loadingLabel"
            text: "Loading"
            horizontalAlignment: HorizontalAlignment.Center
            translationY: 270.0
            textFormat: TextFormat.Plain
            textStyle.base: SystemDefaults.TextStyles.BigText
            textStyle.fontSize: FontSize.Default
            textStyle.textAlign: TextAlign.Center
            textStyle.color: Color.Gray
            visible: false

        }
        ActivityIndicator {
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
            visible: false

        }

    }
}
