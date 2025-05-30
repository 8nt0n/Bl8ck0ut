/*
 * bl8ck0ut Web Server
 * -------------------
 * This project is strictly for educational, research, and demonstration purposes.
 * DO NOT use this code to interfere with wireless communications or violate any laws.
 * Unauthorized transmission on RF bands is illegal and punishable by law.
 * Use responsibly, ethically, and only with explicit permission.
 */


#include <WiFi.h>
#include <WebServer.h>

#define RELAY_PIN 32  // Adjust as needed

WebServer server(80);

bool relayOn = false;
unsigned long relayOffTime = 0;

void handleRoot() {
  // Serve the new cooler HTML page
  server.send(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <title>Bl8ckout Terminal</title>
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Source+Code+Pro&display=swap');

    body {
      background: #000;
      color: #eee;
      font-family: 'Source Code Pro', monospace, monospace;
      margin: 0;
      padding: 2rem;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
      overflow: hidden;
      user-select: none;
    }

    pre.ascii-art {
      color: white;
      font-size: 0.4vw;
      line-height: 0.4vw;
      margin-bottom: 2rem;
      white-space: pre;
      user-select: text;
    }

    .terminal {
      background: #000;
      border: 2px solid #fff;
      border-radius: 8px;
      width: 80%;
      min-height: 700px;
      padding: 1rem;
      display: flex;
      flex-direction: column;
      overflow: hidden;
    }

    .output {
      min-height: 150px;
      padding: 0.5rem;
      border-top: 1px solid rgb(255, 255, 255);
      color: #0f0;
      overflow-y: auto;
      white-space: pre-wrap;
      margin-bottom: 1rem;
      font-size: 14px;
    }

    .input-line {
      display: flex;
      align-items: center;
      color: #0f0;
      font-size: 14px;
    }

    .prompt {
      user-select: none;
      margin-right: 0.5rem;
    }

    #commandInput {
      background: transparent;
      border: none;
      outline: none;
      color: #0f0;
      font-family: 'Source Code Pro', monospace;
      font-size: 14px;
      flex-grow: 1;
    }

    #commandInput::placeholder {
      color: #0a0;
    }

    button {
      display: none;
    }
  </style>
</head>
<body>

  
  <div class="terminal">

    <div style="width: calc(100% + 32px); height: 20px; background-color: #eee; margin: -16px; margin-bottom: 16px; color: black; padding: 1px;">
      Terminal - Bl8ck0ut
    </div>

    <pre class="ascii-art">
            @@@(
                *@@@@@&.
                      &@@@@@@@(
                          #@@@@@@@@/
                              @@@@@@@@@,
                                @@@@@@@@@(
                    ,.             .@@@@@@@@@*
                      /@@@#         /@@@@@@@@@@
                          *@@@@@@@.    @@@@@@@@@@@@
                .&@@@@@@@@@@@@@@@@@@@@&@@@@@@@@@@@@@&
            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        ,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
      @@@@@/.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #@@@@@@@@@@@
    (      @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,    @@@@@@@@
        /@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@(     @@@@@@
        &@@@@@@@@@@@@@@@@@@@@@    *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
      @@@@@@@@@@@@@@@@@@,        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&
      (@@@@@@@@@@@@@@@&        &@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%
      @@@@@@@@@@@@@@@                 %@@@@@@@@@@@@@       #@@@@@@@@@@@@@@@
    @@@@@@@@@@@@@@@                        ,@@@@@@@@            &@@@@@@@@%
    @@@@@@@@@@@@@@                               @@@@@.             .@@@@
    @@/&@@@@@@@@@@                                    @@@              @(
    ,@*  @@@@@@@@@&                                        %.
    //   %@@@@@@@@&
          @@@@@@@@@              &@@@@@@@@@@@@@@@    @@@@            @@@@@@@@@@@@@    %@@@@@@@@@@@  ,@@@@      @@@@@    %@@@@@@@@@@@@@@    @@@@(       .@@@@   @@@@@@@@@@@@@@@@@
              @@@@@@@@           &@@@@      *@@@@@   @@@@           @@@@      %@@@.  /@@@@,         ,@@@@    &@@@@,    *@@@@       #@@@@   @@@@#       .@@@@         @@@@%
                @@@@@@@/          &@@@@       *@@@@   @@@@           @@@@      #@@@,  %@@@@          ,@@@@  *@@@@@      (@@@@       (@@@@   @@@@#       .@@@@         @@@@%
                ,@@@@@@          &@@@@       @@@@@   @@@@           @@@@      &@@@   &@@@@          ,@@@@ @@@@@        #@@@@       (@@@@   @@@@%       .@@@@         @@@@%
                  @@@@@@         &@@@@@@@@@@@@@@     @@@@             @@@@@@@@@@*    &@@@@          ,@@@@@@@@@         (@@@@       (@@@@   @@@@%       .@@@@         @@@@%
                    @@@@@        &@@@@       @@@@@   @@@@           @@@@      @@@@   @@@@@          ,@@@@@@@@@@(       (@@@@       (@@@@   @@@@%       .@@@@         @@@@%
                      @@@@       &@@@@       *@@@@   @@@@           @@@@      #@@@*  @@@@@          ,@@@@@  @@@@@      (@@@@       (@@@@   &@@@@       ,@@@@         @@@@%
                        @@@      &@@@@       &@@@@   @@@@           @@@@      %@@@*  #@@@@          ,@@@@     @@@@@    (@@@@       (@@@@   &@@@@       %@@@@         @@@@%
                          @@     &@@@@@@@@@@@@@@@    @@@@@@@@@@@@#  %@@@@@@@@@@@@@    @@@@@@@@@@@@  ,@@@@      &@@@@@   @@@@@@@@@@@@@@@     @@@@@@@@@@@@@@@          @@@@%
    </pre>


    <div id="output" class="output" aria-live="polite" aria-atomic="false"></div>

    <div class="input-line">
      <span class="prompt">bl8ckout&gt;</span>
      <input id="commandInput" type="text" autocomplete="off" spellcheck="false" placeholder="blackout start -t 100" />
    </div>
  </div>

  <script>
    const output = document.getElementById('output');
    const input = document.getElementById('commandInput');
    
    // Timer state
    let timer = null;

    function appendOutput(text) {
      output.textContent += text + '\n';
      output.scrollTop = output.scrollHeight;
    }

    function parseCommand(cmd) {
      const parts = cmd.trim().split(/\s+/);
      if(parts.length === 0) return;

      if(parts[0] !== 'blackout') {
        appendOutput(`Error: Unknown command '${parts[0]}'. Use 'blackout'.`);
        return;
      }

      if(parts.length < 2) {
        appendOutput('Error: Missing subcommand. Usage: blackout start -t <seconds>');
        return;
      }

      const subcmd = parts[1];

      if(subcmd === 'start') {
        // parse options
        const tIndex = parts.indexOf('-t');
        if(tIndex === -1 || tIndex === parts.length - 1) {
          appendOutput('Error: Missing timer value. Usage: blackout start -t <seconds>');
          return;
        }
        const tValue = parseInt(parts[tIndex + 1], 10);
        if(isNaN(tValue) || tValue <= 0) {
          appendOutput('Error: Invalid timer value.');
          return;
        }
        startTimer(tValue);
      } else if(subcmd === 'status') {
        if(timer) {
          appendOutput('Timer is running...');
        } else {
          appendOutput('Timer is OFF');
        }
      } else if(subcmd === 'stop') {
        stopTimer();
      } else {
        appendOutput(`Error: Unknown subcommand '${subcmd}'.`);
      }
    }



    function startTimer(seconds) {
      if(timer) {
        clearTimeout(timer);
        appendOutput('Previous timer stopped.');
      }
      appendOutput(`Timer started for ${seconds} seconds...`);
      fetch('/start?time=' + seconds).then(() => location.reload());
      timer = setTimeout(() => {
        appendOutput('Timer finished! Blackout done.');
        timer = null;
      }, seconds * 1000);
    }

    function stopTimer() {
      if(timer) {
        clearTimeout(timer);
        timer = null;
        appendOutput('Timer stopped.');
      } else {
        appendOutput('No active timer to stop.');
      }
    }

    input.addEventListener('keydown', (e) => {
      if(e.key === 'Enter') {
        const cmd = input.value;
        appendOutput(`bl8ckout> ${cmd}`);
        parseCommand(cmd);
        input.value = '';
      }
    });

    // Greet user
    appendOutput('Welcome to Bl8ck0ut!');
    appendOutput('Developed by: github.com/8nt0n');
  </script>

</body>
</html>

)rawliteral");
}

void handleStart() {
  if (server.hasArg("time")) {
    int duration = server.arg("time").toInt();
    if (duration > 0) {
      digitalWrite(RELAY_PIN, HIGH);
      relayOn = true;
      relayOffTime = millis() + duration * 1000;
    }
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // Start Access Point
  WiFi.softAP("Bl8ck0ut", "12345678");
  Serial.println("Access Point started!");
  Serial.print("Connect to: ");
  Serial.println("ESP32_Relay");
  Serial.print("Then go to: http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/start", handleStart);
  server.begin();
}

void loop() {
  server.handleClient();

  // WARNING: The below "if statement" controls RF transmission of the slave esp32. The relay control code below is commented out to prevent accidental RF transmission.
  // Uncomment ONLY if you have explicit permission and understand the legal implications.
  // If you’re unsure whether you do or dont: don’t use it.

  /*
  if (relayOn && millis() > relayOffTime) {
    digitalWrite(RELAY_PIN, LOW);
    relayOn = false;
  }
  */
}
