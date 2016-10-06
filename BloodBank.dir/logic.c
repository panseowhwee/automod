// logic.c
// AutoMod 12.6.1 Generated File
// Build: 12.6.1.12
// Model name:	BloodBank
// Model path:	D:\SEM5 mods\IE3110 Simulation\Automod\BloodBank.dir\
// Generated:	Thu Oct 06 09:09:14 2016
// Applied/AutoMod Licensee Confidential
// NO DISTRIBUTION OR REPRODUCTION RIGHTS GRANTED!
// Copyright (c) 1988-2015 Applied Materials All rights reserved.
//
// All Rights Reserved.  Reproduction or transmission in whole or
// in part, in any form or by any means, electronic, mechanical or
// otherwise, is prohibited without the prior written consent of
// copyright owner.
//
// Licensed Material - Property of Applied Materials, Inc.
//
// Applied Materials, Inc.
// 3050 Bowers Drive
// P.O. Box 58039
// Santa Clara, CA 95054-3299
// U.S.A.
//


#include "cdecls.h"


static int32
model_initialize()
{
	{
		{
			create_l(1, am2_L_control, am2_P_start, 0.0);
		}
		{
			create_l(1, am2_L_doctorDown, am2_P_doctorsLunch, 0.0);
		}
		{
			create_l(1, am2_L_recepDown, am2_P_recepLunch, 0.0);
		}
		{
			return 1;
		}
	}
LabelRet: ;
} /* end of model_initialize */

static int32
P_start_arriving(load* this, int32 step, void* args)
{
	void* am_localargs = NULL;
	int32 retval = Continue;
	switch (step) { /* Make the step switcher */
	case Step 1: goto Label1;
	case Step 2: goto Label2;
	default: message("Bad step number %ld.", step);
	}
	retval = Error;
	goto LabelRet;
Label1: ;  /* Step1 */
	{
		{
			while ((ASIclock) <= ToModelTime((7 * 3600), UNITSECONDS)) {
				{
					clone(this, 1, am2_P_formFilling, am2_L_donor);
				}
				{
					if (waitfor(ToModelTime(exponential(am2_stream0, 2), UNITMINUTES), this, P_start_arriving, Step 2, am_localargs) == Delayed)
						return Delayed;
Label2: ; // Step 2
				}
			}
		}
	}
LabelRet: ;
	if (am_localargs)
		xfree(am_localargs);
	return retval;
} /* end of P_start_arriving */

static int32
P_formFilling_arriving(load* this, int32 step, void* args)
{
	void* am_localargs = NULL;
	int32 retval = Continue;
	switch (step) { /* Make the step switcher */
	case Step 1: goto Label1;
	case Step 2: goto Label2;
	case Step 3: goto Label3;
	default: message("Bad step number %ld.", step);
	}
	retval = Error;
	goto LabelRet;
Label1: ;  /* Step1 */
	{
		{
			{
				char* pArg1 = rel_actorname(this, am_model.$sys);
				char* pArg2 = "just arrived at ";
				double pArg3 = FromModelTime((ASIclock / 3600), UNITSECONDS);
				char* pArg4 = " hours";

				message("%s%s%.2lf%s", pArg1, pArg2, pArg3, pArg4);
			}
		}
		{
			pushppa(this, P_formFilling_arriving, Step 2, am_localargs);
			pushppa(this, inqueue, Step 1, am2_Q_formFilling);
			return Continue; // go move into territory
Label2: ; // Step 2
		}
		{
			if (waitfor(ToModelTime(exponential(am2_stream0, 5), UNITMINUTES), this, P_formFilling_arriving, Step 3, am_localargs) == Delayed)
				return Delayed;
Label3: ; // Step 3
		}
		{
			this->nextproc = am2_P_registration; /* send to ... */
			EntityChanged(W_LOAD);
			retval = Continue;
			goto LabelRet;
		}
	}
LabelRet: ;
	if (am_localargs)
		xfree(am_localargs);
	return retval;
} /* end of P_formFilling_arriving */

static int32
P_registration_arriving(load* this, int32 step, void* args)
{
	void* am_localargs = NULL;
	int32 retval = Continue;
	switch (step) { /* Make the step switcher */
	case Step 1: goto Label1;
	case Step 2: goto Label2;
	case Step 3: goto Label3;
	case Step 4: goto Label4;
	case Step 5: goto Label5;
	case Step 6: goto Label6;
	default: message("Bad step number %ld.", step);
	}
	retval = Error;
	goto LabelRet;
Label1: ;  /* Step1 */
	{
		{
			{
				char* pArg1 = rel_actorname(this, am_model.$sys);
				char* pArg2 = "finished filling form at ";
				double pArg3 = FromModelTime((ASIclock / 3600), UNITSECONDS);
				char* pArg4 = " hours";

				message("%s%s%.2lf%s", pArg1, pArg2, pArg3, pArg4);
			}
		}
		{
			pushppa(this, P_registration_arriving, Step 2, am_localargs);
			pushppa(this, inqueue, Step 1, am2_Q_registration_wait);
			return Continue; // go move into territory
Label2: ; // Step 2
		}
		{
			queue* am_locVar;
			AMQueueList* am_locList = NULL;

			ListAppendItem(QueueList, am_locList, &(am2_Q_registration[1]));
			ListAppendItem(QueueList, am_locList, &(am2_Q_registration[2]));
			if (Size(List, _List, am_locList)) {
				int32 am_locVal;
				AMQueueListItem* locIter;
				int i = 0;
				int first = getirand(am2_stream0) % am_locList->numItems;

				for (locIter = am_locList->first; i < first; ++i, locIter = locIter->next) {
				}
				am_locVar = locIter->item;
				am_locVal = QueGetCurConts(ValidPtr(am_locVar, 50, queue*));
				this->attribute->am2_A_regisPointer = am_locVar;
				EntityChanged(0x00000040);
				for (locIter = locIter->next; locIter; locIter = locIter->next) {
					am_locVar = locIter->item;
					if (QueGetCurConts(ValidPtr(am_locVar, 50, queue*)) < am_locVal) {
						this->attribute->am2_A_regisPointer = am_locVar;
						EntityChanged(0x00000040);
						am_locVal = QueGetCurConts(ValidPtr(am_locVar, 50, queue*));
					}
				}
				for (i = 0, locIter = am_locList->first; i < first; ++i, locIter = locIter->next) {
					am_locVar = locIter->item;
					if (QueGetCurConts(ValidPtr(am_locVar, 50, queue*)) < am_locVal) {
						this->attribute->am2_A_regisPointer = am_locVar;
						EntityChanged(0x00000040);
						am_locVal = QueGetCurConts(ValidPtr(am_locVar, 50, queue*));
					}
				}
				ListRemoveAllAndFree(QueueList, am_locList);
			} else
				this->attribute->am2_A_regisPointer = NULL;
				EntityChanged(0x00000040);
		}
		{
			if (this->attribute->am2_A_regisPointer == &(am2_Q_registration[1])) {
				{
					pushppa(this, P_registration_arriving, Step 3, am_localargs);
					pushppa(this, inqueue, Step 1, &(am2_Q_registration[1]));
					return Continue; // go move into territory
Label3: ; // Step 3
				}
				{
					return usefor(&(am2_R_receptionist[1]), 1, this, P_registration_arriving, Step 4, am_localargs, ToModelTime(exponential(am2_stream0, 3), UNITMINUTES));
Label4: ; // Step 4
				}
			}
			else {
				{
					pushppa(this, P_registration_arriving, Step 5, am_localargs);
					pushppa(this, inqueue, Step 1, &(am2_Q_registration[2]));
					return Continue; // go move into territory
Label5: ; // Step 5
				}
				{
					return usefor(&(am2_R_receptionist[2]), 1, this, P_registration_arriving, Step 6, am_localargs, ToModelTime(exponential(am2_stream0, 3), UNITMINUTES));
Label6: ; // Step 6
				}
			}
		}
		{
			this->nextproc = am2_P_medicalScreening; /* send to ... */
			EntityChanged(W_LOAD);
			retval = Continue;
			goto LabelRet;
		}
	}
LabelRet: ;
	if (am_localargs)
		xfree(am_localargs);
	return retval;
} /* end of P_registration_arriving */


typedef struct {
	double freq;
	process** value;
} Oneof0;

static Oneof0 List0[] = {
	{ 90, &am2_P_bloodTest},
	{ 100, &am2_die}
};

static process*
oneofFunc0(load* this)
{
	int ind = 0;
	Oneof0* list = List0;
	double sample = getdrand(am2_stream0) * 100;

	tprintf(tfp, "In oneof\n");
	while (list->freq < sample) {
		ind++;
		list++;
	}
	return *List0[ind].value;
}

static int32
P_medicalScreening_arriving(load* this, int32 step, void* args)
{
	void* am_localargs = NULL;
	int32 retval = Continue;
	switch (step) { /* Make the step switcher */
	case Step 1: goto Label1;
	case Step 2: goto Label2;
	case Step 3: goto Label3;
	case Step 4: goto Label4;
	case Step 5: goto Label5;
	case Step 6: goto Label6;
	default: message("Bad step number %ld.", step);
	}
	retval = Error;
	goto LabelRet;
Label1: ;  /* Step1 */
	{
		{
			{
				char* pArg1 = rel_actorname(this, am_model.$sys);
				char* pArg2 = "just got queue number at ";
				double pArg3 = FromModelTime((ASIclock / 3600), UNITSECONDS);
				char* pArg4 = " hours";

				message("%s%s%.2lf%s", pArg1, pArg2, pArg3, pArg4);
			}
		}
		{
			pushppa(this, P_medicalScreening_arriving, Step 2, am_localargs);
			pushppa(this, inqueue, Step 1, am2_Q_medicalScreening_wait);
			return Continue; // go move into territory
Label2: ; // Step 2
		}
		{
			queue* am_locVar;
			AMQueueList* am_locList = NULL;

			ListAppendItem(QueueList, am_locList, &(am2_Q_medicalScreening[1]));
			ListAppendItem(QueueList, am_locList, &(am2_Q_medicalScreening[2]));
			if (Size(List, _List, am_locList)) {
				int32 am_locVal;
				AMQueueListItem* locIter;
				int i = 0;
				int first = getirand(am2_stream0) % am_locList->numItems;

				for (locIter = am_locList->first; i < first; ++i, locIter = locIter->next) {
				}
				am_locVar = locIter->item;
				am_locVal = QueGetCurConts(ValidPtr(am_locVar, 50, queue*));
				this->attribute->am2_A_screenPointer = am_locVar;
				EntityChanged(0x00000040);
				for (locIter = locIter->next; locIter; locIter = locIter->next) {
					am_locVar = locIter->item;
					if (QueGetCurConts(ValidPtr(am_locVar, 50, queue*)) < am_locVal) {
						this->attribute->am2_A_screenPointer = am_locVar;
						EntityChanged(0x00000040);
						am_locVal = QueGetCurConts(ValidPtr(am_locVar, 50, queue*));
					}
				}
				for (i = 0, locIter = am_locList->first; i < first; ++i, locIter = locIter->next) {
					am_locVar = locIter->item;
					if (QueGetCurConts(ValidPtr(am_locVar, 50, queue*)) < am_locVal) {
						this->attribute->am2_A_screenPointer = am_locVar;
						EntityChanged(0x00000040);
						am_locVal = QueGetCurConts(ValidPtr(am_locVar, 50, queue*));
					}
				}
				ListRemoveAllAndFree(QueueList, am_locList);
			} else
				this->attribute->am2_A_screenPointer = NULL;
				EntityChanged(0x00000040);
		}
		{
			if (this->attribute->am2_A_screenPointer == &(am2_Q_medicalScreening[1])) {
				{
					pushppa(this, P_medicalScreening_arriving, Step 3, am_localargs);
					pushppa(this, inqueue, Step 1, &(am2_Q_medicalScreening[1]));
					return Continue; // go move into territory
Label3: ; // Step 3
				}
				{
					return usefor(&(am2_R_doctor[1]), 1, this, P_medicalScreening_arriving, Step 4, am_localargs, ToModelTime(exponential(am2_stream0, 6), UNITMINUTES));
Label4: ; // Step 4
				}
			}
			else {
				{
					pushppa(this, P_medicalScreening_arriving, Step 5, am_localargs);
					pushppa(this, inqueue, Step 1, &(am2_Q_medicalScreening[2]));
					return Continue; // go move into territory
Label5: ; // Step 5
				}
				{
					return usefor(&(am2_R_doctor[2]), 1, this, P_medicalScreening_arriving, Step 6, am_localargs, ToModelTime(exponential(am2_stream0, 6), UNITMINUTES));
Label6: ; // Step 6
				}
			}
		}
		{
			this->nextproc = oneofFunc0(this); /* send to ... */
			EntityChanged(W_LOAD);
			retval = Continue;
			goto LabelRet;
		}
	}
LabelRet: ;
	if (am_localargs)
		xfree(am_localargs);
	return retval;
} /* end of P_medicalScreening_arriving */


typedef struct {
	double freq;
	process** value;
} Oneof1;

static Oneof1 List1[] = {
	{ 90, &am2_P_donation},
	{ 100, &am2_die}
};

static process*
oneofFunc1(load* this)
{
	int ind = 0;
	Oneof1* list = List1;
	double sample = getdrand(am2_stream0) * 100;

	tprintf(tfp, "In oneof\n");
	while (list->freq < sample) {
		ind++;
		list++;
	}
	return *List1[ind].value;
}

static int32
P_bloodTest_arriving(load* this, int32 step, void* args)
{
	void* am_localargs = NULL;
	int32 retval = Continue;
	switch (step) { /* Make the step switcher */
	case Step 1: goto Label1;
	case Step 2: goto Label2;
	case Step 3: goto Label3;
	case Step 4: goto Label4;
	default: message("Bad step number %ld.", step);
	}
	retval = Error;
	goto LabelRet;
Label1: ;  /* Step1 */
	{
		{
			{
				char* pArg1 = rel_actorname(this, am_model.$sys);
				char* pArg2 = "finished medical screening at ";
				double pArg3 = FromModelTime((ASIclock / 3600), UNITSECONDS);
				char* pArg4 = " hours";

				message("%s%s%.2lf%s", pArg1, pArg2, pArg3, pArg4);
			}
		}
		{
			pushppa(this, P_bloodTest_arriving, Step 2, am_localargs);
			pushppa(this, inqueue, Step 1, am2_Q_bloodTest_wait);
			return Continue; // go move into territory
Label2: ; // Step 2
		}
		{
			pushppa(this, P_bloodTest_arriving, Step 3, am_localargs);
			pushppa(this, inqueue, Step 1, am2_Q_bloodTest);
			return Continue; // go move into territory
Label3: ; // Step 3
		}
		{
			return usefor(am2_R_bpa_bloodTest, 1, this, P_bloodTest_arriving, Step 4, am_localargs, ToModelTime(exponential(am2_stream0, 4), UNITMINUTES));
Label4: ; // Step 4
		}
		{
			this->nextproc = oneofFunc1(this); /* send to ... */
			EntityChanged(W_LOAD);
			retval = Continue;
			goto LabelRet;
		}
	}
LabelRet: ;
	if (am_localargs)
		xfree(am_localargs);
	return retval;
} /* end of P_bloodTest_arriving */

static int32
P_donation_arriving(load* this, int32 step, void* args)
{
	void* am_localargs = NULL;
	int32 retval = Continue;
	switch (step) { /* Make the step switcher */
	case Step 1: goto Label1;
	case Step 2: goto Label2;
	case Step 3: goto Label3;
	case Step 4: goto Label4;
	default: message("Bad step number %ld.", step);
	}
	retval = Error;
	goto LabelRet;
Label1: ;  /* Step1 */
	{
		{
			{
				char* pArg1 = rel_actorname(this, am_model.$sys);
				char* pArg2 = "finished blood test at ";
				double pArg3 = FromModelTime((ASIclock / 3600), UNITSECONDS);
				char* pArg4 = " hours";

				message("%s%s%.2lf%s", pArg1, pArg2, pArg3, pArg4);
			}
		}
		{
			pushppa(this, P_donation_arriving, Step 2, am_localargs);
			pushppa(this, inqueue, Step 1, am2_Q_donation_wait);
			return Continue; // go move into territory
Label2: ; // Step 2
		}
		{
			pushppa(this, P_donation_arriving, Step 3, am_localargs);
			pushppa(this, inqueue, Step 1, am2_Q_donation);
			return Continue; // go move into territory
Label3: ; // Step 3
		}
		{
			clone(this, 1, am2_P_nurseAttending, NULL);
		}
		{
			if (waitfor(ToModelTime(exponential(am2_stream0, 20), UNITMINUTES), this, P_donation_arriving, Step 4, am_localargs) == Delayed)
				return Delayed;
Label4: ; // Step 4
		}
		{
			{
				char* pArg1 = rel_actorname(this, am_model.$sys);
				char* pArg2 = "finished blood donation at ";
				double pArg3 = FromModelTime((ASIclock / 3600), UNITSECONDS);
				char* pArg4 = " hours";

				message("%s%s%.2lf%s", pArg1, pArg2, pArg3, pArg4);
			}
		}
		{
			this->nextproc = am2_die; /* send to ... */
			EntityChanged(W_LOAD);
			retval = Continue;
			goto LabelRet;
		}
	}
LabelRet: ;
	if (am_localargs)
		xfree(am_localargs);
	return retval;
} /* end of P_donation_arriving */

static int32
P_nurseAttending_arriving(load* this, int32 step, void* args)
{
	void* am_localargs = NULL;
	int32 retval = Continue;
	{
	}
LabelRet: ;
	if (am_localargs)
		xfree(am_localargs);
	return retval;
} /* end of P_nurseAttending_arriving */

static int32
P_doctorsLunch_arriving(load* this, int32 step, void* args)
{
	void* am_localargs = NULL;
	int32 retval = Continue;
	switch (step) { /* Make the step switcher */
	case Step 1: goto Label1;
	case Step 2: goto Label2;
	case Step 3: goto Label3;
	case Step 4: goto Label4;
	default: message("Bad step number %ld.", step);
	}
	retval = Error;
	goto LabelRet;
Label1: ;  /* Step1 */
	{
		{
			if (waitfor(ToModelTime(120, UNITMINUTES), this, P_doctorsLunch_arriving, Step 2, am_localargs) == Delayed)
				return Delayed;
Label2: ; // Step 2
		}
		{
			downrsrc(&(am2_R_doctor[1]));
		}
		{
			if (waitfor(ToModelTime(60, UNITMINUTES), this, P_doctorsLunch_arriving, Step 3, am_localargs) == Delayed)
				return Delayed;
Label3: ; // Step 3
		}
		{
			uprsrc(&(am2_R_doctor[1]));
		}
		{
			downrsrc(&(am2_R_doctor[2]));
		}
		{
			if (waitfor(ToModelTime(60, UNITMINUTES), this, P_doctorsLunch_arriving, Step 4, am_localargs) == Delayed)
				return Delayed;
Label4: ; // Step 4
		}
		{
			uprsrc(&(am2_R_doctor[2]));
		}
	}
LabelRet: ;
	if (am_localargs)
		xfree(am_localargs);
	return retval;
} /* end of P_doctorsLunch_arriving */

static int32
P_recepLunch_arriving(load* this, int32 step, void* args)
{
	void* am_localargs = NULL;
	int32 retval = Continue;
	switch (step) { /* Make the step switcher */
	case Step 1: goto Label1;
	case Step 2: goto Label2;
	case Step 3: goto Label3;
	case Step 4: goto Label4;
	default: message("Bad step number %ld.", step);
	}
	retval = Error;
	goto LabelRet;
Label1: ;  /* Step1 */
	{
		{
			if (waitfor(ToModelTime(60, UNITMINUTES), this, P_recepLunch_arriving, Step 2, am_localargs) == Delayed)
				return Delayed;
Label2: ; // Step 2
		}
		{
			this->attribute->am2_A_recep = 1;
			EntityChanged(0x00000040);
		}
		{
			while (this->attribute->am2_A_recep != 3) {
				{
					if (QueGetCurConts(am2_Q_registration_wait) <= 2) {
						{
							downrsrc(&(am2_R_receptionist[ValidIndex("am_model.am_R_receptionist", this->attribute->am2_A_recep, 2)]));
						}
						{
							if (waitfor(ToModelTime(45, UNITMINUTES), this, P_recepLunch_arriving, Step 3, am_localargs) == Delayed)
								return Delayed;
Label3: ; // Step 3
						}
						{
							uprsrc(&(am2_R_receptionist[ValidIndex("am_model.am_R_receptionist", this->attribute->am2_A_recep, 2)]));
						}
						{
							this->attribute->am2_A_recep = this->attribute->am2_A_recep + 1;
							EntityChanged(0x00000040);
						}
					}
					else {
						if (waitfor(ToModelTime(1, UNITMINUTES), this, P_recepLunch_arriving, Step 4, am_localargs) == Delayed)
							return Delayed;
Label4: ; // Step 4
					}
				}
			}
		}
	}
LabelRet: ;
	if (am_localargs)
		xfree(am_localargs);
	return retval;
} /* end of P_recepLunch_arriving */



/* init function for logic.m */
void
model_logic_init(struct model_struct* data)
{
	((ProcSystem*)data->$sys)->modelInitPtr = model_initialize;
	data->am_P_start->aprc = P_start_arriving;
	data->am_P_formFilling->aprc = P_formFilling_arriving;
	data->am_P_registration->aprc = P_registration_arriving;
	data->am_P_medicalScreening->aprc = P_medicalScreening_arriving;
	data->am_P_bloodTest->aprc = P_bloodTest_arriving;
	data->am_P_donation->aprc = P_donation_arriving;
	data->am_P_nurseAttending->aprc = P_nurseAttending_arriving;
	data->am_P_doctorsLunch->aprc = P_doctorsLunch_arriving;
	data->am_P_recepLunch->aprc = P_recepLunch_arriving;
}

