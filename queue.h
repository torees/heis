//
//  queue.h
//  Heisprosjekt
//
//  Created by Kristoffer Krossholm on 05/02/15.
//  Copyright (c) 2015 Kristoffer Krossholm. All rights reserved.
//


int upQueue[4];
int downQueue[4];
int commandQueue[4];

void q_deleteQueue();
void q_addToQueue(int direction, int floor);
void q_removeFromQueue(int direction, int floor);
void q_printQueue();
