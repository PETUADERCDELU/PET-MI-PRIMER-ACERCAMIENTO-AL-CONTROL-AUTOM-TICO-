#include "Headers.h"

// ==================== DEFINICIÓN DE PINES (ARDUINO UNO/UNO R3) ====================
#define PIN_DHT11   2
#define PIN_RELE   13

// ==================== PARÁMETROS Y VARIABLES GLOBALES ====================
float setpoint = 30.0;
float KP = 2.0;
float KI = 0.0;
float KD = 1.0;
double TS = 1.0;          // Tiempo de muestreo por defecto en segundos
double HISTERESIS = 2.0;  // Ancho de banda para el control On/Off

int BTRELE = 100;         // Base de tiempo del relé en ms
float temperaturaActual = 0.0;
unsigned long tiempoUltimoControl = 0;
int porcentajeActuador=0.0;
// ==================== OBJETOS GLOBALES ====================
SensorDeTemperatura SensorDht11(PIN_DHT11);
Rele ReleSsr(PIN_RELE, BTRELE, "SSR");
// Arrancamos con PID habilitado (true) y On/Off deshabilitado (false)
Control sistemaControl(KP, KI, KD, TS, HISTERESIS, true, false);

// ==================== PROTOTIPOS ====================
void procesarComando(String cmd);
void enviarTelemetria();

// ==================== SETUP ====================
void setup() {
    Serial.begin(9600);
  //  Serial.println("Inicio");
    delay(200);
    SensorDht11.inicializar();
    // Apagar el actuador al inicio de forma segura (0% de potencia)
    ReleSsr.Aplicar(1);
    // Nota: Tu clase Rele acota internamente valores menores a 1 como 1%

    tiempoUltimoControl = millis();
    //Serial.println("Sistema de Control Inicializado.");
}

// ==================== LOOP PRINCIPAL ====================
void loop() {
    // 1. Gestión de comandos por puerto serie
 //   temperaturaActual = SensorDht11.leerTemperatura();
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.length() > 0) {
            procesarComando(cmd);
        }
    }

    // 2. Control temporizado por Tiempo de Muestreo (TS)
    unsigned long intervaloMs = (unsigned long)(TS * 1000.0);
    if (millis() - tiempoUltimoControl >= intervaloMs) {
        tiempoUltimoControl = millis(); // Actualizar el marcador de tiempo
        
        // Leer el sensor de temperatura
        float lectura = SensorDht11.leerTemperatura();
        // Validar que la lectura sea correcta antes de aplicar el lazo de control
        if (lectura != -999.0) {
            temperaturaActual = lectura;
            // Calcular la acción de control correspondiente (Internamente evalúa si es PID o ON_OFF)
            double accionControl = sistemaControl.CalcularError(setpoint, temperaturaActual);
            // Mapear y acotar la salida al rango que acepta el Actuador [1, 100]
            porcentajeActuador = (int)accionControl;
            if (porcentajeActuador < 1)   porcentajeActuador = 0;
            if (porcentajeActuador > 100) porcentajeActuador = 100;
            // Aplicar el control al Relé
            ReleSsr.Aplicar(porcentajeActuador);
            // Enviar los datos del sistema
            enviarTelemetria();
        } else {
            delay(200);//Serial.println("[ERROR] No se pudo calcular el control debido a una falla en el sensor.");
        }
    }
       enviarTelemetria();
    delay(10); // Pequeña pausa para estabilidad del microcontrolador
}

// ==================== PROCESAMIENTO DE COMANDOS ====================
void procesarComando(String cmd) {
    cmd.toLowerCase();
    if (cmd.startsWith("setpoint:")) {
        float val = cmd.substring(9).toFloat();
        if (val >= -50 && val <= 300) {
            setpoint = val;
            //Serial.print("[OK] Setpoint cambiado a: ");
            //Serial.print(setpoint, 2);
            //Serial.println(" °C");
        } else { 
            
            delay(2);//Serial.println("[ERROR] Setpoint fuera de rango (-50..300)");
        }
    }
    else if (cmd.startsWith("kp:")) {
        float val = cmd.substring(3).toFloat();
        if (val >= 0 && val <= 255) {
            KP = val;
            // Reiniciamos el objeto de control para aplicar los nuevos coeficientes
            sistemaControl = Control(KP, KI, KD, TS, HISTERESIS, sistemaControl.GetBanderaPID(), sistemaControl.GetBanderaOnOff());
            //Serial.print("[OK] Kp cambiado a: ");
            //Serial.println(KP, 2);
        } else { 
            //Serial.println("[ERROR] Kp fuera de rango (0..255)");
        }
    }
    else if (cmd.startsWith("ki:")) {
        float val = cmd.substring(3).toFloat();
        if (val >= 0 && val <= 100) {
            KI = val;
            sistemaControl = Control(KP, KI, KD, TS, HISTERESIS, sistemaControl.GetBanderaPID(), sistemaControl.GetBanderaOnOff());
            //Serial.print("[OK] Ki cambiado a: ");
            //Serial.println(KI, 2);
        } else { 
            //Serial.println("[ERROR] Ki fuera de rango (0..100)"); 
        }
    }
    else if (cmd.startsWith("kd:")) {
        float val = cmd.substring(3).toFloat();
        if (val >= 0 && val <= 100) {
            KD = val;
            sistemaControl = Control(KP, KI, KD, TS, HISTERESIS, sistemaControl.GetBanderaPID(), sistemaControl.GetBanderaOnOff());
            //Serial.print("[OK] Kd cambiado a: ");
            //Serial.println(KD, 2);
        } else { 
            //Serial.println("[ERROR] Kd fuera de rango (0..100)"); 
        }
    }
    else if (cmd.startsWith("ts:")) {
        float val = cmd.substring(3).toFloat();
        if (val >= 1.0 && val <= 10.0) {
            TS = val;
            sistemaControl = Control(KP, KI, KD, TS, HISTERESIS, sistemaControl.GetBanderaPID(), sistemaControl.GetBanderaOnOff());
            //Serial.print("[OK] Tiempo de muestreo (Ts) cambiado a ");
            Serial.print(TS, 1);
            //Serial.println(" segundos");
        } else { 
         //   /Serial.println("[ERROR] Ts debe estar entre 1 y 10 segundos");
        }
    }
    else if (cmd.startsWith("onoff:on")) {
        sistemaControl.SetBanderaPID(false);
        sistemaControl.SetBanderaOnOff(true);
        //Serial.println("[MODO] Tipo de control cambiado a ON-OFF");
    }
    else if (cmd.startsWith("onoff:off")) {
        sistemaControl.SetBanderaPID(true);
        sistemaControl.SetBanderaOnOff(false);
      //  Serial.println("[MODO] Tipo de control cambiado a PID");
    }
    else {
      delay(2);//  Serial.println("Comandos validos: setpoint:XX, kp:X, ki:X, kd:X, ts:1..10, onoff:on, onoff:off");
    }
}

// ==================== TELEMETRÍA ====================
void enviarTelemetria() {
    String tipoControl = "DESCONOCIDO";
    if (sistemaControl.GetBanderaPID()) {
        tipoControl = "PID";
    } else if (sistemaControl.GetBanderaOnOff()) {
        tipoControl = "ON_OFF";
    }

    // Construir objeto JSON
    String json = "{";
    json += "\"setpoint\":" + String(setpoint, 2) + ",";
    json += "\"temperatura_actual\":" + String(temperaturaActual, 2) + ",";
    json += "\"salida_porcentaje\":" + String((int)porcentajeActuador) + ",";
    json += "\"modo_control\":\"" + tipoControl + "\"";

    if (tipoControl == "PID") {
        json += ",\"parametros_pid\":{";
        json += "\"kp\":" + String(KP, 2) + ",";
        json += "\"ki\":" + String(KI, 2) + ",";
        json += "\"kd\":" + String(KD, 2) + ",";
        json += "\"ts\":" + String(TS, 1);
        json += "}";
    }

    json += "}";

    // Enviar JSON por Serial
    Serial.println(json);
    delay(10);  // Pequeña pausa opcional para asegurar transmisión
}