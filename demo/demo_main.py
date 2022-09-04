import HIFResult

hif_filename = "../HIFSolve/grid7_HIF/HIF.log"
# hifuni_filename = "../HIFUni/grid7_hifuni/hifuni.log"

# mf_filename = "../HIFSolve/grid7_MF/MF.log"
figure_title = 'grid7'

[my_hif_result, totalnum] = HIFResult.ReadIutputs(hif_filename, 3)
# [my_hifuni_result, totalnum] = demo_HIF.ReadOutputsUni(hifuni_filename, 3)
# [my_mf_result, totalmum] = demo_HIF.ReadOutputs(mf_filename, 3)

# demo_HIF.PlotResults_hifmf(my_hif_result, my_mf_result, totalnum, figure_title, 3)
# demo_HIF.PlotResults_HIF_hifuni(my_hif_result, my_hifuni_result, totalnum, figure_title, 3)
HIFResult.PlotResults_HIF(my_hif_result, totalnum, figure_title, 3)
# demo_HIF.PlotResults_hifuni(my_hifuni_result, totalnum, figure_title, 3)


