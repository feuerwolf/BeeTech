
void processTerminalData(){ 
 int  i = 0;
 char stn_out[100] = "";
 char *result_ptr = 0;
 //Serial.println(Buffer_Terminal);

  result_ptr = strstr(Buffer_Terminal,"SETWEIGHT");
  if(result_ptr > 0){
     i = 0;
     while( *result_ptr != ';'){ 
            result_ptr++;
            if(i > 20)
                 break;
     }
     *result_ptr = '*';   
     i = 0;
     while( *result_ptr != ';'){ // find last ; and extract value                  
            result_ptr++; 
            stn_out[i] = *result_ptr; 
            //Serial.print(*result_ptr);                 
            i++;           
            if(i > 20)
                 break;                               
            }                    
     stn_out[i] = 0;
     WEIGHT_OFFSET_float = atof(stn_out);    
     Serial.print("SETWEIGHT = ");
     //Serial.println(stn_out); 
     Serial.println(WEIGHT_OFFSET_float);       
     return;
  }

  result_ptr = strstr(Buffer_Terminal,"SETSERIAL");
  if(result_ptr > 0){
     i = 0;
     while( *result_ptr != ';'){ 
            result_ptr++;
            if(i > 20)
                 break;
     }
     *result_ptr = '*';   
     i = 0;
     while( *result_ptr != ';'){ // find last ; and extract value                  
            result_ptr++; 
            stn_out[i] = *result_ptr; 
            //Serial.print(*result_ptr);                 
            i++;           
            if(i > 20)
                 break;                               
            }                    
     stn_out[i] = 0;
     serialNumber_int = atoi(stn_out);    
     //Serial.print("SERIAL = ");
     //Serial.println(stn_out); 
     //Serial.println(serialNumber_int);
     setEeprom_serialNumber(serialNumber_int);       
     return;
  }

  result_ptr = strstr(Buffer_Terminal,"GETSERIAL");
  if(result_ptr > 0){
     i = getEepromSerialNumber();
     Serial.print("SERIAL EEPROM = ");
     Serial.println(i);    
     Serial.print("SERIAL intern = ");
     Serial.println(serialNumber_int);
     return;    
  }

  

  Serial.println("I dont know this Terminal command");    
}
