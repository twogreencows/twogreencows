//
//  DataProvider.swift
//  Twogreencows
//
//  Created by Laurent Cerveau on 08/25/2021.
//

import Foundation

import Foundation

class DataProvider {
        //A data provider is proving Data to the mobile app. It corresponds to a connection to a TGC server
    var user_uuid:String
    var token:String?
    var server_url:String
    var server_name:String?
    
    init(server_url:String) {    
        self.server_url = server_url
        self.user_uuid = "tmp.user"
    }
    
}
