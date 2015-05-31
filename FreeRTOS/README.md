# Zybo
En este proyecto, se instala el sistema FreeRTOS en la placa Zybo de Digillent, que dispone del chip "Xilinx Zynq Z-7010 AP SoC"

Para ello, se sigue [este tutorial](http://rishifranklin.blogspot.in/2015/04/freertos-on-xilinx-zynq-zybo-single-core.html?m=1) como base, pero en esta misma página se muestran los pasos que se han ido realizando.

##Requisitos
[Base hardware](https://github.com/kharenzze/Zybo/tree/master/zybo_base_system). Esta no es más que la base proporcionada por Digillent, actualizada a la versión 2014.2 de Vivado. Se recuerda que existe un problema con el audio.

En cuanto al software, se toma el de [este](https://github.com/circuitsenses/FreeRTOS-Zybo) proyecto de @circuitsenses, cuyo contenido en este momento (31/05/2015) aparece como zip en este proyecto.

Descargando esta carpeta en el estado actual, se tiene el sistema en funcionamiento con un ejemplo de uso.

##Desarrollo del proyecto
A continuación, se explica el proceso realizado para llegar hasta este punto:

1. Iniciar el proyecto base sobre vivado. Este se encuentra ya compilado
2. Lanzar el SDK
3. Eliminar los anteriores proyectos, a excepcion del hardware_platform_0
4. Crear una nueva aplicacion vacia, con su correspondiente BSP
5. Importar los archivos fuente que se encuentran en: 'FreeRTOS-Zybo-master.zip\FreeRTOS-Zybo-master\zybo_base_system\zybo_base_system\source\vivado\hw\zybo_bsd\zybo_bsd.sdk\FreeRTOSZ\src', a excepcion de lscript.ld
6. Editar el archivo lscript.ld para que la seccion que se muestra quede de la siguiente manera: 

```
SECTIONS
{
.text : {
   *(.freertos_vectors)
   *(.vectors)
   *(.boot)
   *(.text)
   *(.text.*)
   *(.gnu.linkonce.t.*)
   *(.plt)
   *(.gnu_warning)
   *(.gcc_execpt_table)
   *(.glue_7)
   *(.glue_7t)
   *(.vfp11_veneer)
   *(.ARM.extab)
   *(.gnu.linkonce.armextab.*)
} > ps7_ddr_0_S_AXI_BASEADDR
```

*nota: la variación es añadir la linea '*(.freertos_vectors)'

7. Una vez hecho esto, basta con editar el archivo main.c con lo que se desee realizar

##Autores
Este proyecto a sido realizado por 
	Pablo Raez @kharenzze
	Adrian Ripoll @Adrirc8

Para la asignatura 'Programación de Sistemas Empotrados y de Tiempo Real', durante el curso 2014-2015, en la Universidad de Granada. 

