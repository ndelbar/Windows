import { display } from "display";
import document from "document";
import exercise from "exercise";
import { vibration } from "haptics";

const sensors = [];

const speedLabel = document.getElementById("speed-label");
const speedData = document.getElementById("speed-data");
const mainWindow = document.getElementById("main_Window");
const mainWindow2 = document.getElementById("main_Window2");
const topRightIcon = document.getElementById("combo-button-icon");

var myVar = setInterval(myTimer, 1000);

// exercise.start("EUC", { gps: false });

function myTimer() {
  var speed = exercise.stats.speed.current * 2.23694;
  var speed1 = speed.toFixed(1)
  var speed0 = (speed * 10).toFixed(0) | 0;
  var HexColor = speed0.toString(16).toUpperCase();
  if (speed0 > 255)
    {
      HexColor = "FF";
    }
  
  if (HexColor.length == 1)
    {
      HexColor = "0" + HexColor;
    }
  speedData.text = speed1;
  mainWindow.style.fill = "#" + HexColor + "0000";
  mainWindow2.style.fill = "#" + HexColor + "0000";

  if (speed1 > 20)
    {
      vibration.start("ring");
    }
  else
    {
      vibration.stop();
    }
}

let btnTR = document.getElementById("btn-tr");
btnTR.onactivate = function(evt) {
  console.log("TOP RIGHT!");
  if (exercise.state === "started") 
  {
    exercise.stop();
    console.log(topRightIcon);
    topRightIcon.image="start.png";
  }
  else
  {
    exercise.start("cycling", { gps: true });
    topRightIcon.image="stop.png";
  }
}

display.addEventListener("change", () => {
  // Automatically stop all sensors when the screen is off to conserve battery
  // display.on ? sensors.map(sensor => sensor.start()) : sensors.map(sensor => sensor.stop());
});
