//
//  DataProviderManager.swift
//  Twogreencows
//
//  Created by Laurent Cerveau on 08/25/2021.
//

import Foundation
import SystemConfiguration


class DataProviderManager {
        //This is a singleton
    var storageDirectoryURL:URL
    
    init() {
        let docDir = try! FileManager.default.url(for: .documentDirectory, in: .userDomainMask, appropriateFor: nil, create: true)
        self.storageDirectoryURL = docDir
        
        print(docDir)
    }
    
    static let shared:DataProviderManager = {
        let instance = DataProviderManager()
        return instance
    }()
    
    func createDataProvider(serverURL:String?) {
        if let serverURL = serverURL {
            print(serverURL)
        }
    }
}
