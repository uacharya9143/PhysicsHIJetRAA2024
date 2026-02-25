#!/bin/sh

mkdir -p pdf
mkdir -p root

JetR=`DHQuery GlobalSetting.dh Global JetR | tr -d '"'`
#JetR=1 #`DHQuery GlobalSetting.dh Global JetR | tr -d '"'`
Centrality=`DHQuery GlobalSetting.dh Global Centrality | tr -d '"'`
#Centrality="Inclusive"
# Centrality="50to70 70to90"

OrderDHFile=Order.dh

# default for everyone
for R in $JetR
do
   for C in $Centrality
   do
      DHSet $OrderDHFile R${R}_C${C} Order1 int 8
      DHSet $OrderDHFile R${R}_C${C} Order2 int 0
      DHSet $OrderDHFile R${R}_C${C} PinSmall string false
      DHSet $OrderDHFile R${R}_C${C} PinLarge string true
   done
done

# special cases
# DHSet $OrderDHFile R1_C50to90 Order1 int 3
# DHSet $OrderDHFile R1_C50to90 Order2 int 4
##DHSet $OrderDHFile R6_C0to10 PinSmall string true
##DHSet $OrderDHFile R8_C10to30 PinSmall string true
##DHSet $OrderDHFile R8_C10to30 PinSmall string true
##DHSet $OrderDHFile R8_C30to50 PinSmall string true
##DHSet $OrderDHFile R9_C10to30 PinSmall string true

for R in $JetR
do
   for C in $Centrality
   do
      Order1=`DHQuery $OrderDHFile R${R}_C${C} Order1`
      Order2=`DHQuery $OrderDHFile R${R}_C${C} Order2`
      PinSmall=`DHQuery $OrderDHFile R${R}_C${C} PinSmall | tr -d '"'`
      PinLarge=`DHQuery $OrderDHFile R${R}_C${C} PinLarge | tr -d '"'`

      ./ExecuteFit --Input graph/Output_R${R}_Centrality${C}.root \
         --Output pdf/Fit_R${R}_Centrality${C}.pdf \
         --DHFile GlobalSettingTemp.dh --Key R${R}_Centrality${C} \
         --Order1 ${Order1} --Order2 ${Order2} \
         --PinSmall $PinSmall --PinLarge $PinLarge

#      ./ExecuteFit --Input graph/OutputCentralityUp_R${R}_Centrality${C}.root \
#         --Output pdf/FitCentralityUp_R${R}_Centrality${C}.pdf \
#         --DHFile GlobalSettingTemp.dh --Key R${R}_Centrality${C}_CentralityUp \
#         --Order1 ${Order1} --Order2 ${Order2} \
#         --PinSmall $PinSmall --PinLarge $PinLarge

#      ./ExecuteFit --Input graph/OutputCentralityDown_R${R}_Centrality${C}.root \
#         --Output pdf/FitCentralityDown_R${R}_Centrality${C}.pdf \
#         --DHFile GlobalSettingTemp.dh --Key R${R}_Centrality${C}_CentralityDown \
#         --Order1 ${Order1} --Order2 ${Order2} \
#         --PinSmall $PinSmall --PinLarge $PinLarge
   done
done


