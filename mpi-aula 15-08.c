#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    const int PING_PONG_LIMIT = 50;

    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    int number_send, number_received = 0;
    number_send = world_rank;

    if (world_rank == 0)
    {
        MPI_Ssend(&number_send, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);

        printf("Process %d send number %d to process %d\n", world_rank, number_send, world_rank + 1);

        MPI_Recv(&number_received, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process %d received number %d from process %d\n", world_rank, number_received, world_size - 1);
    }
    else if (world_rank == (world_size - 1))
    {
        MPI_Recv(&number_received, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process %d received number %d from process %d\n", world_rank, number_received, world_rank - 1);

        MPI_Ssend(&number_send, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        printf("Process %d send number %d to process %d\n", world_rank, number_send, world_rank + 1);
    }
    else
    {
        MPI_Recv(&number_received, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process %d received number %d from process %d\n", world_rank, number_received, world_rank - 1);

        MPI_Ssend(&number_send, 1, MPI_INT, world_rank, 0, MPI_COMM_WORLD);

        printf("Process %d send number %d to process %d\n", world_rank, number_send, world_rank + 1);
    }

    MPI_Finalize();

    return 0;
}