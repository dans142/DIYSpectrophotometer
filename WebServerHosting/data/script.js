// Complete project details: https://randomnerdtutorials.com/esp8266-nodemcu-plot-readings-charts-multiple/

// Get current sensor readings when the page loads
window.addEventListener('load', getReadings);

// Create Temperature Chart
var chartT = new Highcharts.Chart({
  chart:{
    renderTo:'chart-temperature'
  },
  series: [
    {
      name: 'Violet',
      type: 'line',
      color: '#7F00FF',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#7F00FF',
      }
    },
    {
      name: 'Blue',
      type: 'line',
      color: '#0000FF',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#0000FF',
      }
    },
    {
      name: 'Green',
      type: 'line',
      color: '#008000',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#008000',
      }
    },
    {
      name: 'Yellow',
      type: 'line',
      color: '#FFFF00',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#FFFF00',
      }
    },
    {
      name: 'Orange',
      type: 'line',
      color: '#FFA500',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#FFA500',
      }
    },
    {
      name: 'Red',
      type: 'line',
      color: '#FF0000',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#FF0000',
      }
    },
  ],
  title: {
    text: undefined
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: {
      text: 'LUX'
    }
  },
  credits: {
    enabled: false
  }
});


//Plot temperature in the temperature chart
function plotTemperature(jsonValue) {

  var keys = Object.keys(jsonValue);
  console.log(keys);
  console.log(keys.length);

  for (var i = 0; i < keys.length; i++){
    var x = (new Date()).getTime();
    console.log(x);
    const key = keys[i];
    var y = Number(jsonValue[key]);
    console.log(y);

    if(chartT.series[i].data.length > 40) {
      chartT.series[i].addPoint([x, y], true, true, true);
    } else {
      chartT.series[i].addPoint([x, y], true, false, true);
    }

  }
}

// Function to get current readings on the webpage when it loads for the first time
function getReadings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      plotTemperature(myObj);
    }
  };
  xhr.open("GET", "/readings", true);
  xhr.send();
}

if (!!window.EventSource) {
  var source = new EventSource('/events');

  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('message', function(e) {
    console.log("message", e.data);
  }, false);

  source.addEventListener('new_readings', function(e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);
    plotTemperature(myObj);
  }, false);
}
