import matplotlib.pyplot as plt
import numpy as np


class HIFResult:
    def __init__(self):
        self.dirsize_ = 0  # size on each direction.
        self.matsize_ = 0  # matrix size.
        self.minvtx_ = 0
        self.numlevels_ = 0
        self.tol_ = 0
        self.init_time_ = 0
        self.factor_time_ = 0
        self.sol_time_ = 0
        self.rel_err2_ = 0
        self.sparse_elim_ = 0
        self.skel_ = 0
        self.merge_ = 0


# Read iutputs from filename, d is the dimension.
def ReadIutputs(filename, d):
    num = 0
    dict_result = {}
    infoSize = 0
    infoTime = 0
    with open(filename) as file_input:
        for line in file_input:
            words = line.split()
            if len(words) != 0:
                if words[0] == 'matrixsize':
                    num += 1
                    dict_result[num] = HIFResult()
                    infoSize = 1
                    continue
                if infoSize == 1:
                    infoSize = 0
                    dict_result[num].matsize_ = int(words[0])
                    if d == 2:
                        dict_result[num].dirsize_ = np.sqrt(dict_result[num].matsize_)
                    elif d == 3:
                        dict_result[num].dirsize_ = np.cbrt(dict_result[num].matsize_)
                    dict_result[num].minvtx_ = int(words[1])
                    dict_result[num].numlevels_ = int(words[2])
                    dict_result[num].tol_ = float(words[3])
                if words[0] == 'InitT':
                    infoTime = 1
                    continue
                if words[0] == '0':
                    dict_result[num].sparse_elim_ = float(words[3])
                    dict_result[num].skel_ = float(words[4])
                    dict_result[num].merge_ = float(words[5])
                if infoTime == 1:
                    infoTime = 0
                    dict_result[num].init_time_ = float(words[0])
                    dict_result[num].factor_time_ = float(words[1])
                    dict_result[num].sol_time_ = float(words[2])
                    dict_result[num].rel_err2_ = float(words[3])
    return [dict_result, num]


# Plot results about HIF and MF.
def PlotResults_HIF_MF(hif_result, mf_result, num, titlename, d):
    # plt.rcParams['text.usetex']

    list_size = []

    # Information about HIF.
    list_hif_init_time = []
    list_hif_factor_time = []
    list_hif_sol_time = []
    list_hif_rel_err2 = []

    # Information about MF.
    list_mf_init_time = []
    list_mf_factor_time = []
    list_mf_sol_time = []
    list_mf_rel_err2 = []

    # Add information
    for i in range(1, num + 1):
        list_size.append(hif_result[i].matsize_)

        # HIF
        list_hif_init_time.append(hif_result[i].init_time_)
        list_hif_factor_time.append(hif_result[i].factor_time_)
        list_hif_sol_time.append(hif_result[i].sol_time_)
        list_hif_rel_err2.append(hif_result[i].rel_err2_)

        # MF
        list_mf_init_time.append(mf_result[i].init_time_)
        list_mf_factor_time.append(mf_result[i].factor_time_)
        list_mf_sol_time.append(mf_result[i].sol_time_)
        list_mf_rel_err2.append(mf_result[i].rel_err2_)

    if d == 2:
        #            HIF             MF
        # fact      O(n)        O(n^{3/2})
        # sol       O(n)        O(n log(n))
        # n is the matrix size.

        n = np.array(list_size, dtype='int64')
        logn = np.log(n)
        nlogn = n * logn
        nf3c2 = np.power(n, 1.5)  # fmcn means m/n.

        # Factor time.
        hif_factor_time = np.array(list_hif_factor_time)
        mf_factor_time = np.array(list_mf_factor_time)

        fig1 = plt.figure()

        alpha = hif_factor_time[0] / mf_factor_time[0]
        mf_factor_time = mf_factor_time * alpha
        plt.loglog(list_size, hif_factor_time, '-r', label='HIF')
        plt.loglog(list_size, mf_factor_time, '-b', label='MF')

        alpha = hif_factor_time[0] / n[0]
        n = n * alpha
        plt.plot(list_size, n, '--y', label='$O(n)$')

        alpha = hif_factor_time[0] / nf3c2[0]
        nf3c2 = nf3c2 * alpha
        plt.plot(list_size, nf3c2, '--g', label=r'$O(n^{\frac{3}{2}})$')

        plt.title(f'{titlename}: Factor time')
        plt.xlabel('Matrix size')
        plt.ylabel('Time')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig1.savefig(f'./figures/{titlename}_hifmf_fac.png', format='png')

        # Sol time
        hif_sol_time = np.array(list_hif_sol_time)
        mf_sol_time = np.array(list_mf_sol_time)

        fig2 = plt.figure()

        alpha = hif_sol_time[0] / mf_sol_time[0]
        mf_sol_time = mf_sol_time * alpha
        plt.loglog(list_size, hif_sol_time, '-r', label='HIF')
        plt.loglog(list_size, mf_sol_time, '-b', label='MF')

        alpha = hif_sol_time[0] / n[0]
        n = n * alpha
        plt.plot(list_size, n, '--y', label='$O(n)$')

        alpha = hif_sol_time[0] / nlogn[0]
        nlogn = nlogn * alpha
        plt.plot(list_size, nlogn, '--g', label=r'$O(n \log n)$')

        plt.title(f'{titlename}: Solve time')
        plt.xlabel('Matrix size')
        plt.ylabel('Time')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig2.savefig(f'./figures/{titlename}_hifmf_solve.png', format='png')

        # Error
        hif_rel_err2 = np.array(list_hif_rel_err2)
        mf_rel_err2 = np.array(list_mf_rel_err2)

        fig3 = plt.figure()

        plt.loglog(list_size, hif_rel_err2, '-r', label='HF')
        plt.loglog(list_size, mf_rel_err2, '-b', label='MF')

        plt.title(f'{titlename}: Relative error')
        plt.xlabel('Matrix size')
        plt.ylabel('Relative error')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig3.savefig(f'./figures/{titlename}_hifmf_err2.png', format='png')
    elif d == 3:
        #                HIF            MF
        # fact        O(n logn)       O(n^{2})
        # sol          O(n)           O(n^{4/3})
        # n is the matrix size.

        length = len(list_size)
        n = np.array(list_size, dtype='int64')
        logn = np.log(list_size)
        nlogn = n * logn
        nf4c3 = np.power(n, 4/3)  # fmcn means m/n
        n2 = np.power(n, 2)

        # Factor time.
        hif_factor_time = np.array(list_hif_factor_time)
        mf_factor_time = np.array(list_mf_factor_time)

        fig1 = plt.figure()

        alpha = hif_factor_time[0] / mf_factor_time[0]
        mf_factor_time = mf_factor_time * alpha
        plt.loglog(list_size, hif_factor_time, '-r', label='HIF')
        plt.loglog(list_size, mf_factor_time, '-b', label='MF')

        alpha = hif_factor_time[0] / nlogn[0]
        nlogn = nlogn * alpha
        plt.plot(list_size, nlogn, '--y', label=r'$O(n \log n)$')

        alpha = hif_factor_time[0] / n2[0]
        n2 = n2 * alpha
        plt.plot(list_size, n2, '--g', label=r'$O(n^{2})$')

        plt.title(f'{titlename}: Factor time')
        plt.xlabel('Matrix size')
        plt.ylabel('Time')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig1.savefig(f'./figures/{titlename}_hifmf_fac.png', format='png')

        # Sol time
        hif_sol_time = np.array(list_hif_sol_time)
        mf_sol_time = np.array(list_mf_sol_time)

        fig2 = plt.figure()

        alpha = hif_sol_time[0] / mf_sol_time[0]
        mf_sol_time = mf_sol_time * alpha
        plt.loglog(list_size, hif_sol_time, '-r', label='HIF')
        plt.loglog(list_size, mf_sol_time, '-b', label='MF')

        alpha = hif_sol_time[0] / n[0]
        n = n * alpha
        plt.plot(list_size, n, '--y', label='$O(n)$')

        alpha = hif_sol_time[0] / nf4c3[0]
        nf4c3 = nf4c3 * alpha
        plt.plot(list_size, nf4c3, '--g', label=r'$O(n^{\frac{4}{3}})$')

        plt.title(f'{titlename}: Solve time')
        plt.xlabel('Matrix size')
        plt.ylabel('Time')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig2.savefig(f'./figures/{titlename}_hifmf_solve.png', format='png')

        # Error
        hif_rel_err2 = np.array(list_hif_rel_err2)
        mf_rel_err2 = np.array(list_mf_rel_err2)

        fig3 = plt.figure()

        plt.loglog(list_size, hif_rel_err2, '-r', label='HIF')
        plt.loglog(list_size, mf_rel_err2, '-b', label='MF')

        plt.title(f'{titlename}: Relative error')
        plt.xlabel('Matrix Size')
        plt.ylabel('Relative error')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig3.savefig(f'./figures/{titlename}_hifmf_err2.png', format='png')


# Plot results about HIF with more details.
def PlotResults_HIF(hif_result, num, titlename, d):
    # plt.rcParams['text.usetex']

    list_size = []

    # Information about HIF.
    list_hif_init_time = []
    list_hif_factor_time = []
    list_hif_sol_time = []
    list_hif_rel_err2 = []
    list_hif_sparse_elim = []
    list_hif_skel = []
    list_hif_merge = []

    # add information
    for i in range(1, num + 1):
        list_size.append(hif_result[i].matsize_)

        # HIF
        list_hif_init_time.append(hif_result[i].init_time_)
        list_hif_factor_time.append(hif_result[i].factor_time_)
        list_hif_sol_time.append(hif_result[i].sol_time_)
        list_hif_rel_err2.append(hif_result[i].rel_err2_)
        list_hif_sparse_elim.append(hif_result[i].sparse_elim_)
        list_hif_skel.append(hif_result[i].skel_)
        list_hif_merge.append(hif_result[i].merge_)

    if d == 2:
        #            HIF             MF
        # fact      O(n)        O(n^{3/2})
        # sol       O(n)        O(n log(n))
        # n is the matrix size.

        n = np.array(list_size, dtype='int64')
        nf3c2 = np.power(list_size, 1.5)
        logn = np.log(list_size)
        nlogn = n * logn

        # Initial time.
        hif_init_time = np.array(list_hif_init_time)

        fig0 = plt.figure()

        plt.loglog(list_size, hif_init_time, '-r', label='HIF')

        # If you don't want to re-scaling
        # alpha = 1

        alpha = hif_init_time[0] / n[0]
        n = n * alpha
        plt.plot(list_size, n, '--c', label=r'$O(n)$')

        alpha = hif_init_time[0] / nlogn[0]
        nlogn = nlogn * alpha
        plt.plot(list_size, nlogn, '--m', label=r'$O(n \log n)$')

        plt.title(f'{titlename}: Initial time')
        plt.xlabel('Matrix size')
        plt.ylabel('Time')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig0.savefig(f'./figures/{titlename}_hif_init.png', format='png')

        # Factor time and details.
        hif_factor_time = np.array(list_hif_factor_time)
        hif_sparse_elim = np.array(list_hif_sparse_elim)
        hif_skel = np.array(list_hif_skel)
        hif_merge = np.array(list_hif_merge)

        fig1 = plt.figure()
        plt.loglog(list_size, hif_factor_time, '-r', label='HIF')

        alpha = hif_factor_time[0] / hif_sparse_elim[0]
        hif_sparse_elim = hif_sparse_elim * alpha
        plt.plot(list_size, hif_sparse_elim, '--g', label='SparseElim')

        alpha = hif_factor_time[0] / hif_skel[0]
        hif_skel = hif_skel * alpha
        plt.plot(list_size, hif_skel, '--b', label='Skel')

        alpha = hif_factor_time[0] / hif_merge[0]
        hif_merge = hif_merge * alpha
        plt.plot(list_size, hif_merge, '--y', label='Merge')

        alpha = hif_factor_time[0] / n[0]
        n = n * alpha
        plt.plot(list_size, n, '--c', label='$O(n)$')

        alpha = hif_factor_time[0] / nf3c2[0]
        nf3c2 = nf3c2 * alpha
        plt.plot(list_size, nf3c2, '--m', label=r'$O(n^{\frac{3}{2}})$')

        plt.title(f'{titlename}: Factor time')
        plt.xlabel('Matrix size')
        plt.ylabel('Time')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig1.savefig(f'./figures/{titlename}_hif_fac.png', format='png')

        # Sol time
        hif_sol_time = np.array(list_hif_sol_time)

        fig2 = plt.figure()

        plt.loglog(list_size, hif_sol_time, '-r', label='HIF')

        alpha = hif_sol_time[0] / n[0]
        n = n * alpha
        plt.plot(list_size, n, '--y', label='$O(n)$')

        plt.title(f'{titlename}: Solve time')
        plt.xlabel('Matrix Size')
        plt.ylabel('Time')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig2.savefig(f'./figures/{titlename}_hif_solve.png', format='png')

        # Error
        hif_rel_err2 = np.array(list_hif_rel_err2)
        fig3 = plt.figure()

        plt.loglog(list_size, hif_rel_err2, '-r', label='HF')

        plt.title(f'{titlename}: Relative error')
        plt.xlabel('Matrix size')
        plt.ylabel('Relative error')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig3.savefig(f'./figures/{titlename}_hif_err2.png', format='png')
    elif d == 3:
        #                HIF            MF
        # fact        O(n logn)       O(n^{2})
        # sol          O(n)           O(n^{4/3})
        # n is the matrix size.

        n = np.array(list_size, dtype='int64')
        logn = np.log(list_size)
        nlogn = n * logn
        n2 = n * n
        nf3c2 = np.power(list_size, 1.5)

        # Initial time.
        hif_init_time = np.array(list_hif_init_time)

        fig0 = plt.figure()

        plt.loglog(list_size, hif_init_time, '-r', label='HIF')

        # if you don't want to re-scaling
        # alpha = 1

        alpha = hif_init_time[0] / n[0]
        n = n * alpha
        plt.plot(list_size, n, '--c', label=r'$O(n)$')

        alpha = hif_init_time[0] / nlogn[0]
        nlogn = nlogn * alpha
        plt.plot(list_size, nlogn, '--m', label=r'$O(n \log n)$')

        plt.title(f'{titlename}: Initial time')
        plt.xlabel('Matrix size')
        plt.ylabel('Time')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig0.savefig(f'./figures/{titlename}_hif_init.png', format='png')

        # Factor time.
        hif_factor_time = np.array(list_hif_factor_time)
        hif_sparse_elim = np.array(list_hif_sparse_elim)
        hif_skel = np.array(list_hif_skel)
        hif_merge = np.array(list_hif_merge)

        fig1 = plt.figure()

        plt.loglog(list_size, hif_factor_time, '-r', label='HIF')

        # if you don't want to re-scaling
        # alpha = 1

        alpha = hif_factor_time[0] / hif_sparse_elim[0]
        hif_sparse_elim = hif_sparse_elim * alpha
        plt.plot(list_size, hif_sparse_elim, '-g', label='SparseElim')

        alpha = hif_factor_time[0] / hif_skel[0]
        hif_skel = hif_skel * alpha
        plt.plot(list_size, hif_skel, '-b', label='Skel')

        alpha = hif_factor_time[0] / hif_merge[0]
        hif_merge = hif_merge * alpha
        plt.plot(list_size, hif_merge, '-y', label='Merge')

        alpha = hif_factor_time[0] / nlogn[0]
        nlogn = nlogn * alpha
        plt.plot(list_size, nlogn, '--c', label=r'$O(n \log n)$')

        # alpha = hif_factor_time[0] / n2[0]
        # n2 = n2 * alpha
        # plt.plot(list_size, n2, '--m', label=r'$O(n^2)$')

        alpha = hif_factor_time[0] / nf3c2[0]
        nf3c2 = nf3c2 * alpha
        plt.plot(list_size, nf3c2, '--m', label=r'$O(n^{\frac{3}{2}})$')

        plt.title(f'{titlename}: Factor time')
        plt.xlabel('Matrix size')
        plt.ylabel('Time')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig1.savefig(f'./figures/{titlename}_hif_fac.png', format='png')

        # Sol time
        hif_sol_time = np.array(list_hif_sol_time)

        fig2 = plt.figure()

        plt.loglog(list_size, hif_sol_time, '-r', label='HIF')

        alpha = hif_sol_time[0] / n[0]
        n = n * alpha
        plt.plot(list_size, n, '--y', label='$O(n)$')

        plt.title(f'{titlename}: Solve time')
        plt.xlabel('Matrix Size')
        plt.ylabel('Time')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig2.savefig(f'./figures/{titlename}_hif_solve.png', format='png')

        # Error
        hif_rel_err2 = np.array(list_hif_rel_err2)
        fig3 = plt.figure()

        plt.loglog(list_size, hif_rel_err2, '-r', label='HIF')

        plt.title(f'{titlename}: Relative error')
        plt.xlabel('Matrix size')
        plt.ylabel('Relative error')

        plt.legend()
        plt.tight_layout()
        plt.show()
        fig3.savefig(f'./figures/{titlename}_hif_err2.png', format='png')
