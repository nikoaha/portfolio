# Matomo URL manipulation with JS regex

Track pages with Matomo regardless of the url ID number.

This project was implemented to leave out an ID number that was provided with a certain cloud service's url for matomo's tracking service.

So what we do here:  
Original URL: https://matomo.xxxxx.fi/4  
Edited URL: https://matomo.xxxxx.fi/    <- we left the ID number '4' out

This way we can unify all the same page hits to same folders.
-> The better collective data, the better data analyzing.

## How it's done

We customize the code that is included with every web page that Matomo tracks. (case-sensitive)

For example:

```
<!-- Matomo -->
<script type="text/javascript">
    var _paq = window._paq || [];
    /* tracker methods like "setCustomDimension" should be called before "trackPageView" */
    _paq.push(['trackPageView']);
    _paq.push(['enableLinkTracking']);
    (function () {
        var u = "//matomo.xxxxx.fi/";
        _paq.push(['setTrackerUrl', u + 'matomo.php']);
        _paq.push(['setSiteId', '4']);
        var d = document, g = d.createElement('script'), s = d.getElementsByTagName('script')[0];
        g.type = 'text/javascript'; g.async = true; g.defer = true; g.src = u + 'matomo.js'; s.parentNode.insertBefore(g, s);
    })();
</script>
<!-- End Matomo Code -->

```

...by adding this piece of code after the comment /* tracker method [...] */.

```
var url = location.href;
var reg = /^(?<domain>https:\/\/[^\/]+)(?<siteNum>\/\d+)/;
var getRegUrl = url.replace(url.match(reg).groups.siteNum, "");
```

Eventually it looks like this:

```
<!-- Matomo -->
<script type="text/javascript">
    var _paq = window._paq || [];
    /* tracker methods like "setCustomDimension" should be called before "trackPageView" */
    
    /* START of custom matomo pageviewer code, place before 'trackPageView' */
    var url = location.href;
    var reg = /^(?<domain>https:\/\/[^\/]+)(?<siteNum>\/\d+)/; // locate the url directory int
    var getRegUrl = url.replace(url.match(reg).groups.siteNum, ""); // replace it with nothing
    _paq.push(['setDocumentTitle', 'New Overview']); // for matomo "Behaviour/Page" titles section
    _paq.push(['setCustomUrl', getRegUrl]);
    /* END of custom matomo pageviewer code, place before 'trackPageView' */
    
    _paq.push(['trackPageView']);
    _paq.push(['enableLinkTracking']);
    (function () {

        // Case-sensitive info here - can be found from your own Matomo dashboard.

        var u = "//matomo.xxxxx.fi/"; 
        _paq.push(['setTrackerUrl', u + 'matomo.php']);
        _paq.push(['setSiteId', '4']);
        var d = document, g = d.createElement('script'), s = d.getElementsByTagName('script')[0];
        g.type = 'text/javascript'; g.async = true; g.defer = true; g.src = u + 'matomo.js'; s.parentNode.insertBefore(g, s);
    })();
</script>
<!-- End Matomo Code -->
```

## Documentation

Every row of the code is explained here step-by-step.

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




