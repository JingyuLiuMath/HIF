import HIFResult

hif_filename = "../results/HIFSolve/grid3d7/log/log.log"
# mf_filename = "../results/MFSolve/grid3d7/log/log.log"
figure_title = 'grid3d7'

[my_hif_result, totalnum] = HIFResult.ReadIutputs(hif_filename, 3)
# [my_my_result, totalnum] = HIFResult.ReadIutputs(mf_filename, 3)

HIFResult.PlotResults_HIF(my_hif_result, totalnum, figure_title, 3)
# HIFResult.PlotResults_HIF_MF(my_hif_result, totalnum, figure_title, 3)

