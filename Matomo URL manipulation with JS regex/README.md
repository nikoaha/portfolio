# matomo-custom-tracker

Track pages with Matomo regardless of the provet ID.

We customize the code that is included with every web page that Matomo tracks.

For example:

```
<!-- Matomo -->
<script type="text/javascript">
    var _paq = window._paq || [];
    /* tracker methods like "setCustomDimension" should be called before "trackPageView" */
    _paq.push(['trackPageView']);
    _paq.push(['enableLinkTracking']);
    (function () {
        var u = "//matomo.fns.fi/";
        _paq.push(['setTrackerUrl', u + 'matomo.php']);
        _paq.push(['setSiteId', '4']);
        var d = document, g = d.createElement('script'), s = d.getElementsByTagName('script')[0];
        g.type = 'text/javascript'; g.async = true; g.defer = true; g.src = u + 'matomo.js'; s.parentNode.insertBefore(g, s);
    })();
</script>
<!-- End Matomo Code -->

```

Matomo provides this code for every page. They differ a bit - you can find
your own code provided for each tracked page from Matomo.

In our customized code, the custom code is located in the rows 10 to 16. 
With these seven rows of code, we leave out the provet ID of the URL.

For example:

From this -> https://test.provetcloud.com/4
-> to this -> https://test.provetcloud.com/   <- we left out the ID '4'

This way we can unify all the same page hits to same folders.
-> The better collective data, the better data analyzing.
 
 
-------------------------------------------------------------------------------
 
 
Documentation for the custom code:

Overview:
```
var url = location.href;
var reg = /^(?<domain>https:\/\/[^\/]+)(?<siteNum>\/\d+)/;
var getRegUrl = url.replace(url.match(reg).groups.siteNum, "");
_paq.push(['setDocumentTitle', 'Cloud Overview']);
_paq.push(['setCustomUrl', getRegUrl]);
```

location.href returns the current browser url to variable 'url'.
```
var url = location.href;
```

This regex here ensures that the var 'url' contains a url address.
The url must contain a domain and a number.
```
var reg = /^(?<domain>https:\/\/[^\/]+)(?<siteNum>\/\d+)/;
```

If regex matches, leave out the provet ID integer by replacing it with nothing.
```
var getRegUrl = url.replace(url.match(reg).groups.siteNum, "");
```

Adds a section to matomo 'Page Titles' section. All hits with this code are 
resulted here.
```
_paq.push(['setDocumentTitle', 'Cloud Overview']);
```

Finally, this here adds the hit  to the tracked pages list.
```
_paq.push(['setCustomUrl', getRegUrl]);
```




