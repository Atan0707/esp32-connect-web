#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display

const char *ssid = "arduino-uba-2.4G";
const char *password = "uba12345";

int redPin = 2;
int greenPin = 4;
const int buttonPin = 5;

unsigned long lastIncrementTime = 0; // Stores the last time the counter was incremented
int counter = 0;

int buttonState = 0;

AsyncWebServer server(80);
char data[50]; // Use char array instead of String

void setup()
{
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    lcd.init();
    lcd.backlight();
    lcd.clear();
    Serial.begin(9600);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    delay(5000); // Add delay after WiFi connection
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", data); });

    server.begin();
}

// ...

void loop()
{
    

    //testing counter
    unsigned long currentTime = millis();
    if (currentTime - lastIncrementTime >= 1000) { // Check if a second has passed
        counter++; // Increment the counter
        Serial.print("Counter: ");
        Serial.println(counter);
        lastIncrementTime = currentTime; // Update the last increment time
    }

    buttonState = digitalRead(buttonPin);

    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient httpPost;

        // Send POST request
        httpPost.begin("http://192.168.1.2:3000/postData");       // Specify the URL
        httpPost.addHeader("Content-Type", "application/json"); // Specify the content type

        // Create the JSON object
        DynamicJsonDocument doc(1024);
        doc["value"] = buttonState;
        doc["counter"] = counter; // Add the counter value
        String requestBody;
        serializeJson(doc, requestBody);

        int httpCodePost = httpPost.POST(requestBody); // Send the POST request

        if (httpCodePost > 0)
        {
            String payloadPost = httpPost.getString();
            Serial.println(payloadPost);
        }

        else
        {
            Serial.print("POST request failed with status code ");
            Serial.println(httpCodePost);
        }

        httpPost.end();

//--------------------------------------------------------------------------------------------
        // Send GET request
        HTTPClient httpGet;
        httpGet.begin("http://192.168.1.2:3000/getData");
        int httpCodeGet = httpGet.GET();

        if (httpCodeGet > 0)
        {
            String payloadGet = httpGet.getString();
            Serial.println(payloadGet.c_str());

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payloadGet);

            //extract "value" from the JSON response
            int value = doc["value"];
            snprintf(data, sizeof(data), "%d", value); // Convert integer to string and copy it to data - tak penting pon ni
            Serial.print("Parsed value: ");
            Serial.println(value);
            Serial.println(data);
            
            //extract "counter" from the JSON response
            int counter = doc["counter"];
            Serial.print("Parsed counter: ");
            Serial.println(counter);

            //lcd
            lcd.setCursor(0, 0);
            lcd.print("Value: ");
            lcd.print(value);
            lcd.setCursor(0, 1);
            lcd.print("Counter: ");
            lcd.print(counter);

            if (strcmp(data, "1") == 0) // Use strcmp for comparison
            {
                digitalWrite(2, HIGH);
                digitalWrite(4, LOW);
            }
            else
            {
                digitalWrite(2, LOW);
                digitalWrite(4, HIGH);
            }
        }

        else
        {
            Serial.print("GET request failed with status code ");
            Serial.println(httpCodeGet);
            lcd.print("GET request failed");
        }

        httpGet.end();
    }

  delay(1000);
}
