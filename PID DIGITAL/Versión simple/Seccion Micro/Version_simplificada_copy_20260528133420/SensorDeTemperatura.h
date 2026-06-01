#ifndef SENSOR_DE_TEMPERATURA_H
#define SENSOR_DE_TEMPERATURA_H

#include <DHT.h> // Librería estándar de Adafruit

#define DHTTYPE DHT11 // Definimos el tipo de sensor

class SensorDeTemperatura {
  private:
    DHT _dht;
    int _pin;

  public:
    // Constructor: Configura el pin y el tipo de sensor usando la clase DHT de Adafruit
    SensorDeTemperatura(int pin) : _dht(pin, DHTTYPE), _pin(pin) {}

    // Inicializa el sensor
    void inicializar() {
      _dht.begin();
    }

    // Lee la temperatura y maneja los errores internamente
    float leerTemperatura() {
      float t = _dht.readTemperature();
      
      // Comprobamos si ha habido algún error en la lectura
      if (isnan(t)) {
        //Serial.println("Error obteniendo los datos del sensor DHT11"); 
        return -999.0; // Devolvemos un valor de error controlable
      }
      
      return t;
    }
};

#endif // SENSOR_DE_TEMPERATURA_H