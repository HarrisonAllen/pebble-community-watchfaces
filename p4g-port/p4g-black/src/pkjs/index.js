var CONDITION_KEY = {
    clear: 0,
    cloudy: 2,
    rain: 3,
    snow: 4,
    thunder: 5,
    fog: 6,
    unknown: 7,
    blank: 8
};

var xhrRequest = function (url, type, callback) {
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
        callback(this.responseText);
    };
    xhr.open(type, url);
    xhr.send();
};

function truncateDecimals (num, digits) {
    var numS = num.toString(),
        decPos = numS.indexOf('.'),
        substrLength = decPos == -1 ? numS.length : 1 + decPos + digits,
        trimmedResult = numS.substr(0, substrLength),
        finalResult = isNaN(trimmedResult) ? 0 : trimmedResult;

    return parseFloat(finalResult);
}

var reportCondition = function(code, name, raw){
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open("POST", "http://api.cloudstitch.com/wafulbugs/magic-form/datasources/sheet", true);
    xmlhttp.setRequestHeader("Content-type", "application/json");
    xmlhttp.send(JSON.stringify({"Code": code, "Name": name, "Raw": raw}));
};

function locationSuccess(pos, forced) {
    var latt = pos.coords.latitude;
    var longg = pos.coords.longitude;
    var coordString = truncateDecimals(latt, 1) + ',' + truncateDecimals(longg, 1);
    console.log("coords: " + coordString);
    
    if(forced || localStorage.getItem("last_fetch_date") !== coordString){
        localStorage.setItem("last_fetch_coords", coordString);
        
        var url = "http://api.openweathermap.org/data/2.5/forecast?lat=" + latt + "&lon=" + longg;
        url += "&APPID=bef13b283de23b11e51ebdfb093ef883";
    
        xhrRequest(url, 'GET',
            function (responseText) {
                var json = JSON.parse(responseText);
                console.log("server says: " + JSON.stringify(json));
    
                var conditionId = parseInt(json.list[0].weather[0].id);
                var conditions;
                switch(parseInt(conditionId/100)){
                    case 8:
                        if(conditionId === 800 || conditionId === 801){
                            conditions = CONDITION_KEY.clear;
                        }
                        else{
                            conditions = CONDITION_KEY.cloudy;
                        }
                        break;
                    case 2:
                        conditions = CONDITION_KEY.thunder;
                        break;
                    case 3:
                    case 5:
                        conditions = CONDITION_KEY.rain;
                        break;
                    case 6:
                        conditions = CONDITION_KEY.snow;
                        break;
                    case 7:
                        conditions = CONDITION_KEY.fog;
                        break;
                    case 9:
                        conditions = CONDITION_KEY.unknown;
                        break;
                    default:
                        conditions = CONDITION_KEY.blank;
                        reportCondition(conditionId, json.list[0].weather[0].main, JSON.stringify(json.list[0].weather[0]));
                        break;
                }
                localStorage.setItem("last_fetch_date", new Date());
                localStorage.setItem("last_fetch_conditions", conditions);
                localStorage.setItem("last_fetch_details", conditionId + " " + json.list[0].weather[0].main);
                sendToApp(conditions);
            }
        );
    }
}

function locationError(err) {
    console.log("could not get location");
}

function getWeather() {
    // use cache if last fetch is younger than 15 minutes
    var lastFetchDate = Date.parse(localStorage.getItem("last_fetch_date"));
    var lastFetchConditions = parseInt(localStorage.getItem("last_fetch_conditions"));
    if(lastFetchDate && lastFetchConditions >= 0){
        console.log("using cached conditions: " + lastFetchConditions + " " + localStorage.getItem("last_fetch_details"));
        sendToApp(lastFetchConditions);
        // get new data if last fetch is older than 15 minutes or was a blank 
        if(new Date() - lastFetchDate >= 900000 || lastFetchConditions == 8){
            getNewWeather(true);
        }
        else{
            getNewWeather(false);
        }
    }
    else{
        getNewWeather(true);
    }
}

function getNewWeather(forced){
    navigator.geolocation.getCurrentPosition(
        function(pos){
            locationSuccess(pos, forced);
        },
        locationError,
        {timeout: 10000, maximumAge: 0}
    );
}

function sendToApp(conditions){
    var dictionary = {
        "KEY_CONDITIONS": conditions
    };

    Pebble.sendAppMessage(dictionary,
                          function () {
                              console.log("sent: " + JSON.stringify(dictionary));
                          },
                          function () {
                              console.log("error in sending: " + JSON.stringify(dictionary));
                          }
                         );
}

Pebble.addEventListener('ready', getWeather);

Pebble.addEventListener('appmessage', getWeather);