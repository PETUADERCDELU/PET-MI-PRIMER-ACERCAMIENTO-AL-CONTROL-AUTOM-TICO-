
<img width="1024" height="1042" alt="Gemini_Generated_Image_8juohm8juohm8juo (3)" src="https://github.com/user-attachments/assets/403b56c3-f9db-49ee-b329-ae3f6c3c390c" />

# Mi primer acercamiento al control automático

## Descripción del Proyecto
Este proyecto surge en el marco de las Prácticas Educativas Territoriales (PET) de la [**Facultad de Ciencia y Tecnología (FCyT) - UADER**](https://fcytcdelu.uader.edu.ar/contacto), desarrollado por las cátedras de *Proyecto de Sistemas Digitales* y *Electrónica de Control* de la *Licenciatura en Automatización y Control de Procesos Industriales*.

El objetivo principal es resolver la falta de equipamiento práctico en escuelas técnicas mediante el diseño e implementación de **módulos didácticos de control automático de bajo costo y fácil replicabilidad**. Los módulos permiten experimentar con sistemas de lazo abierto y cerrado utilizando aproximaciones tanto analógicas como digitales.

Todo el material de este repositorio (diseños de hardware, firmware y documentación pedagógica) se distribuye bajo la licencia **GPLv3** para garantizar su acceso público y libre replicación.

---

## Características Técnicas

* **Control Digital:** Implementación de algoritmos de control (PID, On-Off) basados en microcontroladores compatibles con el ecosistema Arduino.
* **Control Analógico:** Módulos de control basados en amplificadores operacionales para el procesamiento de señales continuas.
* **Instrumentación:** Integración de sensores (temperatura LM35, ultrasonido HC-SR04, nivel, posición) y actuadores (motores DC, relés, resistencias calefactoras).
* **Carácter Abierto:** Diseños de PCB y esquemáticos optimizados para manufactura local o prototipado rápido en laboratorios escolares.

---
## Versiones de Implementación

Para cubrir distintos niveles de complejidad, recursos disponibles en las escuelas y objetivos pedagógicos, el proyecto se desarrolla en tres variantes de módulo didáctico:

### 1. PID Digital Extendido (ESP32 + WiFi)
La version extendida es la mas compleja, se busca con ella encontrar el sistema mas completo y mas desarrolado de la idea original, su funcion es permitir a los alumnos trabajar de forma mas directa con el sistema pudiendo aprender de forma mas didactica.

- **Características destacadas:** 
  - 
### 2. PID Digital Simplificado (Arduino)
Esta es la versión simplificada, con ella buscamos poder mostrar el funcionamiento de una manera mas simple y económica, se busca una implementación demostrativa y simple de explicar.

- **Características destacadas:**
  - Arduino Uno: funciona como cerebro del proyecto toma los valores de sensor de temperatura y permite habilitar el paso de la corriente a las resistencias para poder regular la temperatura. 
  - Tranformador: su implementacion es principalmente por seguridad, ya que su labor es principal es expositiva, es preferible trabajar con bajas tensiones.
  - Sensor de temperatura: nos permite saber que temperatura tienen las resitencia en cada momento
  - Bimetalico: en caso de un fallo esto detectara una temperatura inusualmente alta y desconectara el sistema de la red electrica para evitar un daño mayor.
  - Rele de es estado solido: junto con el arduino permite el paso de la corriente a las resistencias.
  - Resistencia: la funcion de estas es generar calor permitir a resto del sistema regular su temperatura

### 3. Versión Analógica (Amplificadores Operacionales)
Para abordar el control desde la perspectiva de la electrónica de señales continuas, esta versión prescinde por completo de microcontroladores. Se basa en circuitos con amplificadores operacionales (como el clásico LM324) configurados como integradores, diferenciadores y amplificadores sumadores.

- **Características destacadas:**
  - Implementación física de las acciones Proporcional, Integral y Derivativa mediante redes de resistencias y capacitores.
  - Permite visualizar en un osciloscopio la señal de control y la realimentación en tiempo real, conectando directamente la teoría de Laplace con la práctica de laboratorio.
  - Resulta ideal para explicar conceptos como el "windup" del integrador o el efecto del ruido en la derivada sin la abstracción del código digital.
  - Totalmente independiente de baterías o fuentes externas complejas (se alimenta con fuente simétrica básica), garantizando su replicabilidad en talleres con recursos limitados.
