//---------------------------------------CREATE LOAD(DONOR)--------------------------------------------
begin model initialization function
	create 1 load of type L_control to P_start
	create 1 load of type L_doctorDown to P_doctorsLunch
	create 1 load of type L_recepDown to P_recepLunch
	return true
end

begin P_start arriving
	while (absolute clock)<=(7*3600) do begin
		clone 1 load to P_formFilling nlt L_donor
		wait for e 2 min
	end
end

//--------------------------------------MODELING PROCESSES--------------------------------------------		
/*
This process simulates the form filling process of donor
*/
begin P_formFilling arriving
	print this load "just arrived at " (absolute clock/3600) as .2 " hours" to message
	move into Q_formFilling 
	wait for e 5 min 	//to insert val

	send to P_registration
end

/*
This process simulates the registration process at the counter with 1/2 servers
Start: When donors place their forms in the tray
End: When queue number is collected
*/
begin P_registration arriving
	print this load "finished filling form at " (absolute clock/3600) as .2 " hours" to message
	move into Q_registration_wait
	
	choose a queue from among Q_registration(1), Q_registration(2) whose 
		current loads is minimum save choice as A_regisPointer
	
	if A_regisPointer = Q_registration(1)
		begin 
			move into Q_registration(1)
			use R_receptionist(1) for e 3 min
		end
	else
		begin 
			move into Q_registration(2)
			use R_receptionist(2) for e 3 min
		end
	
	send to P_medicalScreening
end

/*
This process simulates the medical screening process in 2 rooms with 2 doctors
Start: When donors enter the room
End: When donors exit the room
*/
begin P_medicalScreening arriving
	print this load "just got queue number at " (absolute clock/3600) as .2 " hours" to message
	move into Q_medicalScreening_wait
	
	choose a queue from among Q_medicalScreening(1), Q_medicalScreening(2) whose 
		current loads is minimum save choice as A_screenPointer
	
	if A_screenPointer = Q_medicalScreening(1)
		begin 
			move into Q_medicalScreening(1)
			use R_doctor(1) for e 6 min
		end
	else
		begin 
			move into Q_medicalScreening(2)
			use R_doctor(2) for e 6 min
		end
		 

	send to oneof(90:P_bloodTest, 10:die)  //to add in prob of failing medical screening
end

/*
This process simulates the blood test process in 1 room with 1 staff
Start: When donors enter the room
End: When donors exit the room
*/
begin P_bloodTest arriving
	print this load "finished medical screening at " (absolute clock/3600) as .2 " hours" to message
	move into Q_bloodTest_wait
	move into Q_bloodTest
	use R_bpa_bloodTest for e 4 min
	//else if R_bpa_bloodTest is down then use R_staff(1) for 4 min //simulate break time
	
	send to oneof(90:P_donation, 10:die)
	
end


begin P_donation arriving
	print this load "finished blood test at " (absolute clock/3600) as .2 " hours" to message
	move into Q_donation_wait
	move into Q_donation
	wait for e 20 min //to insert val
	print this load "finished blood donation at " (absolute clock/3600) as .2 " hours" to message
	send to die

end

//---------------------------------------BREAK(DOWN) TIMES MODELLING----------------------------------

begin P_doctorsLunch arriving
	wait for 120 min
	take down R_doctor(1)	
	wait for 60 min
	bring up R_doctor (1)
	take down R_doctor(2)
	wait for 60 min
	bring up R_doctor(2)
end

begin P_recepLunch arriving
	wait for 60 min
	set A_recep to 1
	while A_recep != 3 do begin
		if Q_registration_wait current loads <= 2 then begin
			take down R_receptionist(A_recep)
			wait for 45 min
			bring up R_receptionist(A_recep)
			set A_recep to A_recep+1
		end
		else
			wait for 1 min
	end
end
		
//-------------------------------------OTHER FUNCTIONS ---------------------------------------------

