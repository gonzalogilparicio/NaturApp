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

// creo instancias

CTBot bot;

// declaración de variables y sus pines

int nivelMax1 = 2;
int nivelMax2 = 4;
int nivelMinPaludario = 6;
int nivelMinBidon = 8;
int bombaLlenado = 9;

void setup()
{
	Serial.begin(9600);

	// inicia conexion wifi

	Serial.println("Iniciando...");
	bot.wifiConnect("", ""); // VER ACA DE QUE SI NO CONECTA QUE SIGA INTENTANDO!!!!

	// inicia telegram bot

	bot.setTelegramToken("");

	if (bot.testConnection())
	{
		Serial.println("Telegram bot conectado");
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

void loop()
{
	// lectura de los estados de los sensores

	int estadoNivelMax1 = digitalRead(nivelMax1);
	int estadoNivelMax2 = digitalRead(nivelMax2);
	int estadoNivelMinPaludario = digitalRead(nivelMinPaludario);
	int estadoNivelMinBidon = digitalRead(nivelMinBidon);

	// muestra en monitor serie los estados actuales de cada sensor

	Serial.println("Nivel Max 1: ", estadoNivelMax1);
	Serial.println("Nivel Max 2: ", estadoNivelMax2);
	Serial.println("Nivel Min Paludario: ", estadoNivelMinPaludario);
	Serial.println("Nivel Min Bidon: ", estadoNivelMinBidon);

	// condiciones

	// si ambos sensores de niveles maximos de llenado estan en 0, el sensor de nivel minimo de
	// paludario está en 0 y el sensor de nivel minimo de bidon está en 1, entonces empieza a llenar
	// avisando del evento por telegram

	if (estadoNivelMax1 == 0 && estadoNivelMax2 == 0 && estadoNivelMinPaludario == 0 && estadoNivelMinBidon == 1)
	{
		digitalWrite(bombaLlenado, HIGH);
		Serial.println("Llenando paludario");
		bot.sendMessage(1249478693, "Llenando paludario");
	}

	// si alguno de los sensores de niveles maximos de llenado cambia
	// su estado a 1, el llenado se frena y manda aviso por telegram

	else if (estadoNivelMax1 == 1 || estadoNivelMax2 == 1)
	{
		digitalWrite(bombaLlenado, LOW);
		Serial.println("Llenado finalizado");
		bot.sendMessage(1249478693, "Llenado finalizado");
	}

	// si el bidon está vacio, el llenado no se inicia y manda alerta por telegram

	else if (estadoNivelMinBidon == 0)
	{
		Serial.println("Bidon de agua vacio");
		digitalWrite(bombaLlenado, LOW);
		bot.sendMessage(1249478693, "Bidon de agua vacio");
	}

	// si no ocurre ninguna de las situaciones anteriores, entonces
	// el paludario tiene agua y la bomba permanece apagada

	else
	{
		digitalWrite(bombaLlenado, LOW);
	}

	delay(1000); // probar iteraciones con este segundo y con 10 o 20 a ver que pasa
}