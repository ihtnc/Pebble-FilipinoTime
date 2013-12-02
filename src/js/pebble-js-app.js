var enable_logging = true;
var include_holiday = 1;
var enable_blink = 1;
var dynamic_font_size = 1;
var bt_notification = 0;
var invert_screen = 0;
var count_up_cutover = 40;

Pebble.addEventListener("ready", function() {
	var temp_holiday = localStorage.getItem("IncludeHoliday");
	if (temp_holiday) { 
		include_holiday = temp_holiday; 
		
		if(enable_logging) console.log("Pebble.ready: IncludeHoliday=" + include_holiday);
	}
	else {
		if(enable_logging) console.log("Pebble.ready: default IncludeHoliday=" + include_holiday);
	}

	var temp_blink = localStorage.getItem("EnableBlink");
	if (temp_blink) { 
		enable_blink = temp_blink; 
		
		if(enable_logging) console.log("Pebble.ready: EnableBlink=" + enable_blink);
	}
	else {
		if(enable_logging) console.log("Pebble.ready: default EnableBlink=" + enable_blink);
	}

	var temp_font = localStorage.getItem("DynamicFontSize");
	if (temp_font) { 
		dynamic_font_size = temp_font; 
		
		if(enable_logging) console.log("Pebble.ready: DynamicFontSize=" + dynamic_font_size);
	}
	else {
		if(enable_logging) console.log("Pebble.ready: default DynamicFontSize=" + dynamic_font_size);
	}

	var temp_bt = localStorage.getItem("BTNotification");
	if (temp_bt) { 
		bt = temp_bt; 
		
		if(enable_logging) console.log("Pebble.ready: BTNotification=" + bt);
	}
	else {
		if(enable_logging) console.log("Pebble.ready: default BTNotification=" + bt);
	}

	var temp_invert = localStorage.getItem("InvertScreen");
	if (temp_invert) { 
		invert_screen = temp_invert; 
		
		if(enable_logging) console.log("Pebble.ready: InvertScreen=" + invert_screen);
	}
	else {
		if(enable_logging) console.log("Pebble.ready: default InvertScreen=" + invert_screen);
	}

	var temp_count = localStorage.getItem("CountUpCutover");
	if (temp_count) { 
		count_up_cutover = temp_count; 
		
		if(enable_logging) console.log("Pebble.ready: CountUpCutover=" + count_up_cutover);
	}
	else {
		if(enable_logging) console.log("Pebble.ready: default CountUpCutover=" + count_up_cutover);
	}
});

Pebble.addEventListener("showConfiguration", function(e) {
	var json = 
		"[" +
			"{" +
				"'caption':'Show Holidays'," +
				"'key':'IncludeHoliday'," +
				"'initialValue':'" + include_holiday + "'," +
				"'type':'checkbox'" +
			"}," +
			"{" +
				"'caption':'Blink on Change'," +
				"'key':'EnableBlink'," +
				"'initialValue':'" + enable_blink + "'," +
				"'type':'checkbox'" +
			"}," +
			"{" +
				"'caption':'Autofit Text'," +
				"'key':'DynamicFontSize'," +
				"'initialValue':'" + dynamic_font_size + "'," +
				"'type':'checkbox'" +
			"}," +
			"{" +
				"'caption':'BT Notification'," + 
				"'key':'BTNotification'," +
				"'type':'checkbox'," +
				"'initialValue':'" + bt_notification + "'" +
			"}," +
			"{" +
				"'caption':'Invert Screen Color'," + 
				"'key':'InvertScreen'," +
				"'type':'checkbox'," +
				"'initialValue':'" + invert_screen + "'" +
			"}," +
			"{" +
				"'caption':'Start countdown on'," + 
				"'key':'CountUpCutover'," +
				"'initialValue':'" + count_up_cutover + "'" +
			"}" +
		"]";
	var url = "http://ihtnc-pebble-config.azurewebsites.net/?";
	var title = "&title=Filipino+Time+Configuration";
	var fields = "&fields=" + encodeURIComponent(json);
	
	if(enable_logging) console.log("Pebble.showConfiguration: url=" + url + title + fields);
	Pebble.openURL(url + title + fields);
});

Pebble.addEventListener("webviewclosed", function(e) {
	if(!e.response && enable_logging) {
		console.log("Pebble.webviewclosed: no response received");
		return;
	}
	else {
		console.log("Pebble.webviewclosed: response=" + e.response);
	}

	var configuration = JSON.parse(e.response);
	if(configuration["action"] == "cancel") {
		if(enable_logging) console.log("Pebble.webviewclosed: action=cancel");
		return;
	}
	
	if(enable_logging) console.log("Pebble.webviewclosed: action=save");

	if(configuration["IncludeHoliday"] == null) include_holiday = 0;
	else include_holiday = configuration["IncludeHoliday"];
	
	localStorage.setItem("IncludeHoliday", include_holiday);
	if(enable_logging) console.log("Pebble.webviewclosed: include_holiday=" + include_holiday);

	if(configuration["EnableBlink"] == null) enable_blink = 0;
	else enable_blink = configuration["EnableBlink"];

	localStorage.setItem("EnableBlink", enable_blink);
	if(enable_logging) console.log("Pebble.webviewclosed: enable_blink=" + enable_blink);

	if(configuration["DynamicFontSize"] == null) dynamic_font_size = 0;
	else dynamic_font_size = configuration["DynamicFontSize"];

	localStorage.setItem("DynamicFontSize", dynamic_font_size);
	if(enable_logging) console.log("Pebble.webviewclosed: dynamic_font_size=" + dynamic_font_size);
	
	if(configuration["BTNotification"] == null) bt_notification = 0;
	else bt_notification = configuration["BTNotification"];

	localStorage.setItem("BTNotification", bt_notification);
	if(enable_logging) console.log("Pebble.webviewclosed: bt_notification=" + bt_notification);	

	if(configuration["InvertScreen"] == null) invert_screen = 0;
	else invert_screen = configuration["InvertScreen"];

	localStorage.setItem("InvertScreen", invert_screen);
	if(enable_logging) console.log("Pebble.webviewclosed: invert_screen=" + invert_screen);	

	if(configuration["CountUpCutover"] != null) count_up_cutover = configuration["CountUpCutover"];
	localStorage.setItem("CountUpCutover", count_up_cutover);
	if(enable_logging) console.log("Pebble.webviewclosed: count_up_cutover=" + count_up_cutover);	

	//since thinCFG returne everything as string, convert the values we retrieved to int before sending to the pebble watchface
	configuration["IncludeHoliday"] = parseInt(include_holiday);
	configuration["EnableBlink"] = parseInt(enable_blink);
	configuration["DynamicFontSize"] = parseInt(dynamic_font_size);
	configuration["BTNotification"] = parseInt(bt_notification);
	configuration["InvertScreen"] = parseInt(invert_screen);
	configuration["CountUpCutover"] = parseInt(count_up_cutover);
	Pebble.sendAppMessage(configuration);
	
	if(enable_logging) console.log("Pebble.sendAppMessage: done");
});
