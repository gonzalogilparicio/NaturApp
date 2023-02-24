// programa para llenar un paludario cuando el
// agua llega a un determinado nivel
//
// componentes usados:
// Arduino Mega 2560
// Sensor 6017-1p
// Sensor 5csa-p45
// Relay 1CH-TTL
// Bomba de agua 12v
//
// autor: gonzalo gil paricio
// version: 1.0.0
// repositorio: https://github.com/gonzalogilparicio/paludario

int nivelMax1 = 2;
int nivelMax2 = 4;
int nivelMinPaludario = 6;
int nivelMinBidon = 8;
int bombaLlenado = 10;

void setup()
{
  Serial.println("Iniciando...");
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

  Serial.println(estadoNivelMax1);
  Serial.println(estadoNivelMax2);
  Serial.println(estadoNivelMinPaludario);
  Serial.println(estadoNivelMinBidon);

  // condiciones

  // si ambos sensores de niveles maximos de llenado estan en 0, el sensor de nivel minimo de
  // paludario está en 0 y el sensor de nivel minimo de bidon está en 1, entonces empieza a llenar
  // avisando del evento por telegram

  if (estadoNivelMax1 == 0 && estadoNivelMax2 == 0 && estadoNivelMinPaludario == 0 && estadoNivelMinBidon == 1)
  {
    digitalWrite(bombaLlenado, HIGH);
    Serial.println("Llenando paludario");
    // MANDAR TELEGRAM O MAIL
  }

  // si alguno de los sensores de niveles maximos de llenado cambia
  // su estado a 1, el llenado se frena y manda aviso por telegram

  else if (estadoNivelMax1 == 1 || estadoNivelMax2 == 1)
  {
    digitalWrite(bombaLlenado, LOW);
    Serial.println("Llenado finalizado");
    // MANDAR TELEGRAM O MAIL
  }

  // si el bidon está vacio, el llenado no se inicia y manda alerta por telegram

  else if (estadoNivelMinBidon == 0)
  {
    Serial.println("Bidon de agua vacio");
    digitalWrite(bombaLlenado, LOW);
    // MANDAR TELEGRAM O MAIL
  }

  // si no ocurre ninguna de las situaciones anteriores, entonces
  // el paludario tiene agua y la bomba permanece apagada

  else
  {
    digitalWrite(bombaLlenado, LOW);
  }

  delay(1000);
}
