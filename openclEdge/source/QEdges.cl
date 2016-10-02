//
//  QEdges.cl
//  kernel for edge calculation
//
//  Created by Quan Yuan on 7/23/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//


//#define MIN_EDGE_VALUE  4
//#define BLOCK_SIZE 8
//#define NUM_TOP_EDGE_POINTS
//
//
//void get_line_featrue_vector(unsigned char* im, unsigned w, unsigned h, unsigned block_len);
//    
//void get_linets_in_a_block(unsigned * edge_block, unsigned w, unsigned block_len, std::vector<std::vector<int>> & edges, unsigned edge_th){
//        unsigned edge_sum = get_edge_sum_in_a_block(edge_block, w, block_len);
//        if(edge_sum < edge_th)
//            return;
//        
//}
//
//

kernel void get_edge_sum_in_a_block(global unsigned char * edge_block,
                                    global size_t* rowBytes,
                                    global size_t* block_len,
                                    global size_t *esum){
        *esum = 0;
        for(size_t i = 0; i < *block_len; i++){
            for(size_t j = 0; j< *block_len; j++)
                *esum += edge_block[i*(*rowBytes)+j];
        }
    
}
    
