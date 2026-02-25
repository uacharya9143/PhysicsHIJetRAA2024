#!/bin/bash


PPChoice=2025317_PP
PbPbChoice=2025317_PbPb

#for i in Input/$PPChoice/*root Input/$PPChoice/*txt Input/$PbPbChoice/*root Input/$PbPbChoice/*txt
#for i in Input/$PbPbChoice/*root Input/$PbPbChoice/*txt
#do
#   j=`basename $i`
#   j=${j/EOY}
#   j=${j/Eta13}
#   cp $i Input/$j
#done


#//////////////////////PP//////////////////////////////////

# #echo "Running pp... (using files from $PPChoice)"
 for i in Input/PP*Fine*root
 do
    ./Execute --Input $i --Output Output_R4/`basename ${i/Fine}` --DHFile GlobalSetting.dh \
       --DoZeroing false --DoEarthquake false --DoBias false
 done

#////////////////////////////////////////////////////////

#/////////////////////////////////PbPb//////////////////////////////
#echo "Running PbPb... (using files from $PbPbChoice)"
for i in Input/PbPb*Fine*root
do
   ./Execute --Input $i --Output Output_R4/`basename ${i/Fine}` --DHFile GlobalSetting.dh \
      --DoZeroing false --DoEarthquake false --DoBias false
done

#////////////////////////////////////////////////////////


#echo "Running cleaned check..."
#for i in Input/*NominalFine.root
#do
#   ./Execute --Input $i --Output Output/`basename ${i/Fine/Cleaned}` --DHFile GlobalSetting.dh \
#      --DoZeroing true --ZeroMin 0.33 --ZeroMax 3.00 --DoEarthquake false --DoBias false
#   ./Execute --Input $i --Output Output/`basename ${i/Fine/CleanedStrong}` --DHFile GlobalSetting.dh \
#      --DoZeroing true --ZeroMin 0.50 --ZeroMax 2.00 --DoEarthquake false --DoBias false
#done

#echo "Running earthquake check..."
#for i in Input/*NominalFine.root
#do
#   ./Execute --Input $i --Output Output/`basename ${i/Fine/Earthquake}` --DHFile GlobalSetting.dh \
#      --DoZeroing false --DoEarthquake true --EarthquakeMagnitude 1.0 --DoBias false
#done

#echo "Running bin bias check..."
#for i in Input/*NominalFine.root
#do
 #  ./Execute --Input $i --Output Output/`basename ${i/Fine/BinBiasUp}` --DHFile GlobalSetting.dh \
 #     --DoZeroing false --DoEarthquake false --DoBias true --GenBiasPower 1.0 --RecoBiasPower 0.0
 #  ./Execute --Input $i --Output Output/`basename ${i/Fine/BinBiasDown}` --DHFile GlobalSetting.dh \
 #     --DoZeroing false --DoEarthquake false --DoBias true --GenBiasPower -1.0 --RecoBiasPower 0.0
#done


