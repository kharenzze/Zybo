# Zybo
En este proyecto, se instala el sistema FreeRTOS en la placa Zybo de Digillent, que dispone del chip "Xilinx Zynq Z-7010 AP SoC"

Para ello, se sigue [este tutorial](http://rishifranklin.blogspot.in/2015/04/freertos-on-xilinx-zynq-zybo-single-core.html?m=1) como base, pero en esta misa página se muestran los pasos que se han ido realizando.

En primer lugar, hay que crear e instalar la configuración hardware de la placa. Para ello, se toma el que se encuentra en la siguiente dirección:
[Base hardware](https://github.com/kharenzze/Zybo/tree/master/zybo_base_system). Esta no es mas que la base proporcionada por Digillent, actualizada a la versión 2014.2 de Vivado. Se recuerda que existe un problema con el audio.

En cuanto al software, se toma el de [este](https://github.com/circuitsenses/FreeRTOS-Zybo) proyecto de @circuitsenses, cuyo contenido en este momento (31/05/2015) aparece como zip en este proyecto.

Xilinx Vivado 2014.2
