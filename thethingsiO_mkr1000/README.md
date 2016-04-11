  <div id="readme" class="readme blob instapaper_body">
    <article class="markdown-body entry-content" itemprop="text"><h1><a id="user-content-thethingsio-Arduino-MKR1000-library" class="anchor" href="#thethingsio-Arduino-MKR1000-library" aria-hidden="true"><svg aria-hidden="true" class="octicon octicon-link" height="16" role="img" version="1.1" viewBox="0 0 16 16" width="16"><path d="M4 9h1v1h-1c-1.5 0-3-1.69-3-3.5s1.55-3.5 3-3.5h4c1.45 0 3 1.69 3 3.5 0 1.41-0.91 2.72-2 3.25v-1.16c0.58-0.45 1-1.27 1-2.09 0-1.28-1.02-2.5-2-2.5H4c-0.98 0-2 1.22-2 2.5s1 2.5 2 2.5z m9-3h-1v1h1c1 0 2 1.22 2 2.5s-1.02 2.5-2 2.5H9c-0.98 0-2-1.22-2-2.5 0-0.83 0.42-1.64 1-2.09v-1.16c-1.09 0.53-2 1.84-2 3.25 0 1.81 1.55 3.5 3 3.5h4c1.45 0 3-1.69 3-3.5s-1.5-3.5-3-3.5z"></path></svg></a>thethings.iO-Arduino-MKR1000-library</h1>

<p>Arduino libraries and examples to connect to <a href="http://thethings.iO">http://thethings.iO</a></p>


<p>If you want to use the WiFi client you should <code>#include &lt;SPI.h&gt;</code> <code>#include &lt;WiFi101.h&gt;</code> before including this library.Then include this library with <code>#include &lt;thethingsiO_mkr1000.h&gt;</code>. Before using any client from this library, the WiFI connection should be initialized. Check the examples for mor details</p>

<div class="highlight highlight-source-c++"><pre><span class="pl-c">
  Serial.println("Connecting MKR1000 One to network...");
   // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED ) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    WiFi.begin(WIFI_AP, WIFI_PWD);
    // wait 10 seconds for connection:
    delay(5000);
    status = WiFi.status();
    status = LWiFi.status();
</pre></div>



<div class="highlight highlight-source-c++"><pre><span class="pl-en">thethingsiOWiFi();
thethingsiOWiFi (String token);
</pre></div>

<p>Create a new client. The first form is used to activate new things with the <code>activate(String)</code> method. The second form sets the token for an already activated thing.</p>

<div class="highlight highlight-source-c++"><pre>String <span class="pl-en">activate</span>(String activationCode);</pre></div>

<p>Activates a thing with the given activation code. The token for the Thing is automatically set and returned. In the case of failure an empty string is returned.</p>

<div class="highlight highlight-source-c++"><pre>String <span class="pl-en">getToken</span>();</pre></div>

<p>Returns the token associated with this thing.</p>

<div class="highlight highlight-source-c++"><pre><span class="pl-k">void</span> <span class="pl-en">addValue</span>(String key, String value);
<span class="pl-k">void</span> <span class="pl-en">addValue</span>(String key, <span class="pl-k">double</span> value);</pre></div>

<p>Adds the key/value pair in the buffer of data to be sent. Since those pairs are stored in memory until the data is sent the user should be aware of the memory.</p>

<div class="highlight highlight-source-c++"><pre>String <span class="pl-en">send</span>();</pre></div>

<p>Send and flush the data stored in the data buffer (added with the <code>addValue</code> functions).</p>

<div class="highlight highlight-source-c++"><pre>String <span class="pl-en">read</span>(String key);
String <span class="pl-en">read</span>(String key, <span class="pl-k">int</span> limit);</pre></div>

<p>Get the last <code>limit</code> key/value pairs sent to this thing with the given key. In the first form , the last 10 items are returned.</p>

<div class="highlight highlight-source-c++"><pre><span class="pl-k">void</span> <span class="pl-en">subscribe</span>();
<span class="pl-k">int</span> <span class="pl-en">available</span>();
String <span class="pl-en">read</span>();</pre></div>

<p>Subscribe, check for data to be read and actually read the data from the thing. This is used to listen and get the data sent to this thing.</p>
</article>
  </div>

</div>

</article>
  </div>

</div>

 </body>
</html>
