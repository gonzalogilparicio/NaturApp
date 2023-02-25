// NaturApp
//
// Programa para llenar un paludario cuando el
// agua llega a un determinado nivel
// integración con telegram bot para generar alertas
//
// Componentes usados:
// Arduino Mega 2560
// Sensor 6017-1p
// Sensor 5csa-p45
// Relay 1CH-TTL
// Bomba de agua 12v
// ESP32
//
// autor: Gonzalo Gil Paricio
// version: 1.0.0
// repositorio público: https://github.com/gonzalogilparicio/NaturApp

// librerias

#include <CTBot.h>
#include <WiFi.h>

// creo instancias

CTBot bot;

// declaración de variables y sus pines

const int nivelMax1 = 36;
const int nivelMax2 = 39;
const int nivelMinPaludario = 34;
const int nivelMinBidon = 35;
const int bombaLlenado = 13;
const int telegramUserID = 1249478693;

void setup()
{
	Serial.begin(115200);

	// inicia conexion wifi

	Serial.println("Iniciando programa");
	int attempts = 0;
	while (WiFi.status() != WL_CONNECTED && attempts < 10)
	{
		attempts++;
		Serial.println("Intentando conectar a WiFi...");
		bot.wifiConnect("");
		delay(1000);
	}

	if (WiFi.status() == WL_CONNECTED)
	{
		Serial.println("WiFi conectado");
		Serial.println("Dirección IP asignada: ");
		Serial.println(WiFi.localIP());
	}
	else
	{
		Serial.println("No se pudo conectar a WiFi");
	}

	// inicia telegram bot

	bot.setTelegramToken("");

	if (bot.testConnection())
	{
		Serial.println("Telegram bot conectado");
		bot.sendMessage(telegramUserID, "Conexión con telegram establecida");
	}
	else
	{
		Serial.println("Telegram bot sin conexión");
	}

	// configuro el comportamiento de cada pin

	pinMode(nivelMax1, INPUT);
	pinMode(nivelMax2, INPUT);
	pinMode(nivelMinPaludario, INPUT);
	pinMode(nivelMinBidon, INPUT);
	pinMode(bombaLlenado, OUTPUT);
}

// funcion de llenado

void llenarPaludario()
{
	Serial.println("Llenando paludario");
	bot.sendMessage(telegramUserID, "Llenando paludario");

	while (digitalRead(nivelMax1) == 0 && digitalRead(nivelMax2) == 0 && digitalRead(nivelMinBidon) == 0)
	{
		digitalWrite(bombaLlenado, HIGH);
		delay(100);
	}

	digitalWrite(bombaLlenado, LOW);
	Serial.println("Llenado finalizado");
	bot.sendMessage(telegramUserID, "Llenado finalizado");
}

void loop()
{
	Serial.println("Arrancando loop");

	int valorNM1 = digitalRead(nivelMax1);
	int valorNM2 = digitalRead(nivelMax2);
	int valorNMP = digitalRead(nivelMinPaludario);
	int valorNMB = digitalRead(nivelMinBidon);

	String msgValores = "Nivel Max 1: " + String(valorNM1) + "\nNivel Max 2: " + String(valorNM2) + "\nNivel Min Paludario: " + String(valorNMP) + "\nNivel Min Bidon: " + String(valorNMB);

	// muestra en monitor serie los estados actuales de cada sensor

	Serial.println(msgValores);

	// condiciones

	// si ambos sensores de niveles maximos de llenado estan en 0, el sensor de nivel minimo de
	// paludario está en 0 y el sensor de nivel minimo de bidon está en 1, entonces empieza a llenar
	// avisando del evento por telegram

	if (digitalRead(nivelMax1) == 0 && digitalRead(nivelMax2) == 0 && digitalRead(nivelMinPaludario) == 1 && digitalRead(nivelMinBidon) == 0)
	{
		llenarPaludario();
	}
	// si el bidon está vacio, el llenado no se inicia y manda alerta por telegram

	if (digitalRead(nivelMinBidon) == 1)
	{
		digitalWrite(bombaLlenado, LOW);
		Serial.println("Bidon de agua vacio");
		bot.sendMessage(telegramUserID, "Bidon de agua vacio");
	}

	// si no ocurre ninguna de las situaciones anteriores, entonces
	// el paludario tiene agua y la bomba permanece apagada

	Serial.println("Llegué al final del loop");
	delay(5000); // probar iteraciones con este segundo y con 10 o 20 a ver que pasa
}