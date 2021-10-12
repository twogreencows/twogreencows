//
//  BaseObject.swift
//  Twogreencows
//
//  Created by Laurent Cerveau on 08/22/2021.
//

import Foundation

class BaseObject {
    var uuid:String?;
    var objectVersion:Int = 0;
    var dataVersion:Int = 0;
    
    init() {
        uuid="nil-uuid";
        objectVersion = 0;
        dataVersion = 0;
    }
    
}
