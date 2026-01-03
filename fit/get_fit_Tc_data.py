L = [8,16,32,64,128,256,512,1024,2048,4096,8192]

L = [8,16,32,64,128,256,512,1024,2048,4096]

# sigma = '1.250'
# beta_l =  0.29634
# beta_h =  0.29642

# sigma = '1.500'
# beta_l =  0.31265
# beta_h =  0.31277

sigma = '1.750'
beta_l =  0.329085
beta_h =  0.329175


sigma = '1.875'
beta_l =  0.33692
beta_h =  0.33703


sigma = '2.000'
beta_l =  0.344395
beta_h =  0.344485

sigma = '2.500'
beta_l =  0.369395
beta_h =  0.369505


flag = 0
with open('fit_data/fitting_' + sigma,'w') as wf:
    for i in L:
        # with open('../../data_O1/cmp/cmp_L_' + str(i) + '_sigma_' + sigma + '.dat','r') as rf:
        with open('../data_beta_c_fit/cmp/cmp_L_' + str(i) + '_sigma_' + sigma + '.dat','r') as rf:
            context = ""
            if flag == 0 :
                for j in range(2):  # skip header
                    context = context + rf.readline()
                flag = 1
            else :
                for j in range(2):  # skip header
                    rf.readline()
                
            
            while True:
                text = rf.readline()    
                check = text.split()
                if check==[]: break     # if in the end of text, exit
                beta_now = float(check[3])
                # print(beta_now)
                if(beta_now >= beta_l and beta_now <= beta_h):
                    context = context + text
            wf.write(context.replace('x',' '))
