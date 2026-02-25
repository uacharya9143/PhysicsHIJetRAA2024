#source /cvmfs/cms-bril.cern.ch/cms-lumi-pog/brilws-docker/brilws-env
#export PATH=$HOME/.local/bin:/cvmfs/cms.cern.ch/cms-lumi-pog/brilconda/bin:$PATH


Prepare:
	rm -f Input
	ln -sf $(ProjectBase)/MainAnalysis/23762_UnfoldPreparation//Output Input
	ln -sf $(ProjectBase)/MainAnalysis/GlobalSetting/GlobalSetting.dh

RunPP:
	for i in Input/PPData_R*NominalFine_JSON.txt; do bash CheckJSON_pp.sh $$i; done
#	 for i in Input/PPData_R1_CentralityInclusive_NominalFine_JSON.txt; do bash CheckJSON.sh $$i; done

RunPPTierCheck:
	# for i in Input/FlatMiniAOD/PPData_R*NominalFine_JSON.txt; do bash CheckJSON.sh $$i MiniAOD.dh; done
	# for i in Input/FlatAOD/PPData_R*NominalFine_JSON.txt; do bash CheckJSON.sh $$i AOD.dh; done

RunAA:
	for i in Input/PbPbData_R*NominalFine_JSON.txt; do bash CheckJSON.sh $$i; done



