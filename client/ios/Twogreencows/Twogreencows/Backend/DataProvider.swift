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
    var user_username:String
    var token:String?
    var server_url:String
    
    init(user_username:String, token:String?) {
        self.user_username = user_username
        self.user_uuid = "usr-"+user_username
        self.token = token
        self.server_url = "0.0.0.0:3000"
    }
    
}
