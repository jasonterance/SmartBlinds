//
//  ViewController.swift
//  SmartBlinds
//
//  Created by Jason Terance on 9/11/20.
//  Copyright © 2020 JTM2. All rights reserved.
//

//



import UIKit
import CoreBluetooth


private var centralManager: CBCentralManager!
private var peripheral: CBPeripheral!
private var redChar: CBCharacteristic?
private var greenChar: CBCharacteristic?
private var blueChar: CBCharacteristic?
private var LEDMODEChar: CBCharacteristic?
private var ALARMSTATUSChar: CBCharacteristic?
private var ALARMHOURChar: CBCharacteristic?
private var ALARMMINChar: CBCharacteristic?
private var CURRENTHOURChar: CBCharacteristic?
private var CURRENTMINChar: CBCharacteristic?
private var BLINDSChar: CBCharacteristic?
class Peripheral: NSObject {

    

    public static let ACSUUID     = CBUUID.init(string: "4fafc201-1fb5-459e-8fcc-c5c9c331914b")
    public static let redLEDCharacteristicUUID   = CBUUID.init(string: "a8f5b648-cf10-11ea-87d0-0242ac130003")
    public static let greenLEDCharacteristicUUID   = CBUUID.init(string: "a8f5b756-cf10-11ea-87d0-0242ac130003")
    public static let blueLEDCharacteristicUUID   = CBUUID.init(string: "a8f5b81e-cf10-11ea-87d0-0242ac130003")
    public static let ALARMSTATUSCharacteristicUUID   = CBUUID.init(string: "beb5483e-36e1-4688-b7f5-ea07361b26a8")
    public static let LEDMODECharacteristicUUID   = CBUUID.init(string: "a8f5b418-cf10-11ea-87d0-0242ac130003")
    public static let ALARMHOURCharacteristicUUID   = CBUUID.init(string: "a8f5b8dc-cf10-11ea-87d0-0242ac130003")
    public static let ALARMMINCharacteristicUUID   = CBUUID.init(string: "a8f5bb8e-cf10-11ea-87d0-0242ac130003")


    public static let CURRENTHOURUUID =  CBUUID.init(string: "97764282-db42-11ea-87d0-0242ac130003")

     public static let CURRENTMINUUID =  CBUUID.init(string: "97764386-db42-11ea-87d0-0242ac130003")
    public static let BLINDSUUID =  CBUUID.init(string: "97763f12-db42-11ea-87d0-0242ac130003")
   

}

class ViewController: UIViewController, CBPeripheralDelegate, CBCentralManagerDelegate {
    
    override func viewDidLoad() {
        super.viewDidLoad()
        centralManager = CBCentralManager(delegate: self, queue: nil)
        print("YESSIRSKI")
        BTSTATUS.startAnimating()
        
        
    }

    func writeValuetoChar(withCharacteristic characteristic: CBCharacteristic, withValue data: String){
      let valueString = (data as NSString).data(using: String.Encoding.utf8.rawValue)
        
              peripheral.writeValue(valueString!, for: characteristic, type: CBCharacteristicWriteType.withResponse)
            }
        
  
    
    private var centralManager: CBCentralManager!
       private var peripheral: CBPeripheral!
    @IBOutlet weak var CONNECT: UIButton!
    
    @IBOutlet weak var ALARM: UIDatePicker!

    @IBOutlet weak var RESETTIME: UIButton!
    @IBAction func TIMERESET(_ sender: Any) {
        
        
               
        let date = Date()
         let calendar = Calendar.current
        let hour2 = calendar.component(.hour, from: date)
        
        writeValuetoChar(withCharacteristic: CURRENTHOURChar!, withValue: String(describing: hour2))
        let minutes2 = calendar.component(.minute, from: date)
        writeValuetoChar(withCharacteristic: CURRENTMINChar!, withValue: String(describing: minutes2))
        
    }
    
    @IBAction func ALARMCHANGE(_ sender: Any) {
        
   ALARM.datePickerMode = .time
       
     let date = ALARM.date
     let components = Calendar.current.dateComponents([.hour, .minute], from: date)
        let hour:Int = Int(components.hour!)
        let minute:Int = Int(components.minute!)
        let hour1: String = String(describing: hour)
        let minute1: String = String(describing: minute)
        print(hour, minute)
        
        writeValuetoChar( withCharacteristic: ALARMHOURChar!, withValue: hour1)
        writeValuetoChar( withCharacteristic: ALARMMINChar!, withValue: minute1)
        
}
    
    
   
    
    @IBOutlet weak var LEDMODE: UISegmentedControl!
    
    @IBAction func LEDMODECHANGE(_ sender: Any) {
       print("LED MODE",LEDMODE.selectedSegmentIndex);
       let ctrl:Int = Int(LEDMODE.selectedSegmentIndex)
        let ctrl1:String = String(describing: ctrl)
       writeValuetoChar( withCharacteristic: LEDMODEChar!, withValue: ctrl1)
        }

    
    @IBOutlet weak var REDLED: UISlider!
    
    @IBAction func REDSlide(_ sender: Any) {
        print("red:",REDLED.value);
               let redslider:Int = Int(REDLED.value)
               let redslider1:String = String(describing: redslider)
        writeValuetoChar( withCharacteristic: redChar!, withValue: redslider1)

           }
    @IBOutlet weak var GREENLED: UISlider!
    
    @IBAction func GREENSLIDE(_ sender: Any) {
        print("GREEN:",GREENLED.value);
        let greenslider:Int = Int(GREENLED.value)
        let greenslider1:String = String(greenslider)
        writeValuetoChar( withCharacteristic: greenChar!, withValue: greenslider1)
    }
    @IBOutlet weak var BLUELED: UISlider!
    
    @IBAction func BLUESLIDE(_ sender: Any) {
        print("BLUE:",BLUELED.value);
        let blueslider:UInt8 = UInt8(BLUELED.value)
        let blueslider1:String = String( describing: blueslider)
         writeValuetoChar( withCharacteristic: blueChar!, withValue: blueslider1)
}
    
    @IBAction func ASCHANGE(_ sender: Any) {
        print("ALARM STATUS",ALARMSTATUS.state);
        
        if ALARMSTATUS.isOn == true{
            ALARM.isEnabled = true
            writeValuetoChar( withCharacteristic: ALARMSTATUSChar!, withValue: "1" )
            let date = ALARM.date
            let components = Calendar.current.dateComponents([.hour, .minute], from: date)
               let hour:Int = Int(components.hour!)
               let minute:Int = Int(components.minute!)
               let hour1: String = String(describing: hour)
               let minute1: String = String(describing: minute)
               print(hour, minute)
               
               writeValuetoChar( withCharacteristic: ALARMHOURChar!, withValue: hour1)
               writeValuetoChar( withCharacteristic: ALARMMINChar!, withValue: minute1)
        }
        else
        {
            ALARM.isEnabled = false
            writeValuetoChar( withCharacteristic: ALARMSTATUSChar!, withValue: "0" )
        }
        
        
    }
    
    @IBOutlet weak var ALARMSTATUS: UISwitch!
    
   
    
    
    
    
        
    
    
    @IBOutlet weak var BTSTATUS: UIActivityIndicatorView!
    
    @IBOutlet weak var OPEN: UIButton!
    var button: Int = 0
    @IBAction func OPENPRESS(_ sender: Any) {
        
            writeValuetoChar( withCharacteristic: BLINDSChar!, withValue: "1")
       
    }

    
   
    
    
    
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
                print("Central state update")
                if central.state != .poweredOn {
                    print("Central is not powered on")
                } else {
                    print("Central scanning for", Peripheral.ACSUUID);
                   
                    centralManager.scanForPeripherals(withServices: [Peripheral.ACSUUID],
                            options: [CBCentralManagerScanOptionAllowDuplicatesKey : true])
                }
            }
            
            
            
            func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {

                // We've found it so stop scan
                self.centralManager.stopScan()

                // Copy the peripheral instance
                self.peripheral = peripheral
                self.peripheral.delegate = self

                // Connect!
                self.centralManager.connect(self.peripheral, options: nil)

            }

        func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
                  if peripheral == self.peripheral {
                    print(peripheral.name! + " Connected")
                      BTSTATUS.stopAnimating()
                    BTSTATUS.isHidden = true
                    peripheral.discoverServices([Peripheral.ACSUUID])
                  }
              }
    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        if peripheral == self.peripheral {
            print("Disconnected")
            
            REDLED.isEnabled = false
            GREENLED.isEnabled = false
            BLUELED.isEnabled = false
            LEDMODE.isEnabled = false
            ALARMSTATUS.isEnabled = false
            ALARM.isEnabled = false
            BTSTATUS.isHidden = false
            OPEN.isEnabled = false
            RESETTIME.isEnabled = false
            BTSTATUS.startAnimating()
            
            REDLED.value = 0
            GREENLED.value = 0
            BLUELED.value = 0
            
            self.peripheral = nil
            
            // Start scanning again
            print("Central scanning for", Peripheral.ACSUUID);
            centralManager.scanForPeripherals(withServices: [Peripheral.ACSUUID],
                                              options: [CBCentralManagerScanOptionAllowDuplicatesKey : true])
        }
    }
            func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
                if let services = peripheral.services {
                    for service in services {
                        if service.uuid == Peripheral.ACSUUID {
                            print("ACS Service found")
                            BTSTATUS.stopAnimating()
                            
                            peripheral.discoverCharacteristics([ Peripheral.redLEDCharacteristicUUID, Peripheral.greenLEDCharacteristicUUID,
                            Peripheral.blueLEDCharacteristicUUID,
                            Peripheral.LEDMODECharacteristicUUID,
                            Peripheral.ALARMSTATUSCharacteristicUUID,
                            Peripheral.ALARMHOURCharacteristicUUID,
                            Peripheral.ALARMMINCharacteristicUUID,
                            Peripheral.BLINDSUUID,
                            Peripheral.CURRENTMINUUID,
                            Peripheral.CURRENTHOURUUID
                            ], for: service)
                            return
                        }
                    }
                }
            }

    func peripheral(_ peripheral: CBPeripheral,
                     didUpdateNotificationStateFor characteristic: CBCharacteristic,
                     error: Error?) {
        print("Enabling notify ", characteristic.uuid)
        
        if error != nil {
            print("Enable notify error")
        }
    }

    

           func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
                      if let characteristics = service.characteristics {
                          for characteristic in characteristics {
                              if characteristic.uuid == Peripheral.redLEDCharacteristicUUID {
                                  print("Red LED characteristic found")
                                  redChar = characteristic
                                  REDLED.isEnabled = true
                              }
                              else if characteristic.uuid == Peripheral.greenLEDCharacteristicUUID {
                                  print("Green LED characteristic found")
                                  greenChar = characteristic
                                  GREENLED.isEnabled = true
                              }
                                else if characteristic.uuid == Peripheral.blueLEDCharacteristicUUID {
                                  print("Blue LED characteristic found");
                                  blueChar = characteristic
                                  BLUELED.isEnabled = true
                              }
                                else if characteristic.uuid == Peripheral.LEDMODECharacteristicUUID {
                                print("LEDMODE characteristic found");
                                LEDMODEChar = characteristic
                                LEDMODE.isEnabled = true
                            }
                                else if characteristic.uuid == Peripheral.ALARMSTATUSCharacteristicUUID {
                                print("ALARM STATUS characteristic found");
                                ALARMSTATUSChar = characteristic
                                ALARMSTATUS.isEnabled = true
                            }
                            else if characteristic.uuid == Peripheral.ALARMHOURCharacteristicUUID {
                            print("ALARM HOUR characteristic found");
                            ALARMHOURChar = characteristic
                                ALARM.isEnabled = true
                            }
                            else if characteristic.uuid == Peripheral.ALARMMINCharacteristicUUID {
                            print("ALARM MIN characteristic found");
                            ALARMMINChar = characteristic
                                ALARM.isEnabled = true
                          }
                            else if characteristic.uuid == Peripheral.BLINDSUUID {
                            print("BLINDS characteristic found");
                            BLINDSChar = characteristic
                                
                                OPEN.isEnabled = true
                        }
                            else if characteristic.uuid == Peripheral.CURRENTHOURUUID {
                            print("HOUR characteristic found");
                            RESETTIME.isEnabled = true
                                CURRENTHOURChar = characteristic
                                
                                       
                                let date = Date()
                                 let calendar = Calendar.current
                                let hour2 = calendar.component(.hour, from: date)
                                
                                writeValuetoChar(withCharacteristic: CURRENTHOURChar!, withValue: String(describing: hour2))
                               
                                   }
                                
                               
                        
                            else if characteristic.uuid == Peripheral.CURRENTMINUUID {
                            print("MIN characteristic found");
                            CURRENTMINChar = characteristic
                                let date = Date()
                                let calendar = Calendar.current
                                let minutes2 = calendar.component(.minute, from: date)
                                writeValuetoChar(withCharacteristic: CURRENTMINChar!, withValue: String(describing: minutes2))
                                
                      }
                  }
            }
}
}

