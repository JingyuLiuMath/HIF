import HIFResult

hif_filename = "E:/FASTsolver/new_results/HIFSolve/grid3d7/log/log.log"
mf_filename = "E:/FASTsolver/new_results/MFSolve/grid3d7/log/log.log"

figure_title = 'grid3d7'

[my_hif_result, hif_totalnum] = HIFResult.ReadInputs(hif_filename, 3)
[my_mf_result, mf_totalmum] = HIFResult.ReadInputs(mf_filename, 3)

HIFResult.PlotResults_HIF_MF(my_hif_result, my_mf_result, hif_totalnum, mf_totalmum, figure_title, 3)
# HIFResult.PlotResults_HIF(my_hif_result, hif_totalnum, figure_title, 3)
