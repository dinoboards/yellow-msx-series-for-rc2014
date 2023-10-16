#ifndef PARTITION_SELECTION_H
#define PARTITION_SELECTION_H

#include <partition.h>

#define PARTITIONS_PER_PAGE 15

extern partitionInfo partitions[MAX_PARTITIONS_TO_HANDLE];
extern uint32_t      autoPartitionSizeInK;

void goPartitioningMainMenuScreen(void);

#endif
