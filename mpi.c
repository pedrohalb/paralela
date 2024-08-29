#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){
    const int PING_PONG_LIMIT = 50;

    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    if(world_size != 2){
        fprintf(stderr,"World size must be two for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int ping_pong_count = 0;
    int parter_rank=(world_rank + 1) % 2;
    while (ping_pong_count < PING_PONG_LIMIT)
    {
        if(world_rank == ping_pong_count % 2){
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, parter_rank, 0, MPI_COMM_WORLD);
            printf("%d send and incremented ping_pong_count %d to %d\n", world_rank, ping_pong_count, parter_rank);
        } else {
            MPI_Recv(&ping_pong_count, 1, MPI_INT, parter_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d received ping_pong_count %d from %d\n", world_rank, ping_pong_count, parter_rank);

        }
    }

    MPI_Finalize();

    return 0;
}