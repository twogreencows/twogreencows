//
//  BaseObject.swift
//  Twogreencows
//
//  Created by Laurent Cerveau on 08/22/2021.
//

import Foundation

class BaseObject {
    var uuid:String?;
    var objectVersion:Int?;
    var dataVersion:Int?;
    
    init() {
        uuid="nil-uuid";
        objectVersion = 0;
        dataVersion = 0;
    }
    
}
