//
//  NetworkBonjourListener.swift
//  Twogreencows
//
//  Created by Laurent Cerveau on 08/27/2021.
//

import Foundation
import Network

class NetworkBonjourListener {
    
    var browser:NWBrowser
    
    init() {
        let parameters = NWParameters()
        parameters.includePeerToPeer = true
        self.browser = NWBrowser(for: .bonjour(type: "_twogreencowsagent._tcp", domain: nil), using: parameters)
    }
    
    static let shared:NetworkBonjourListener = {
        let instance = NetworkBonjourListener()
        return instance
    }()
    
    func startBonjourListener() {
        browser.stateUpdateHandler = { newState in
                    print("browser.stateUpdateHandler \(newState)")
        }
        browser.browseResultsChangedHandler = { results, changes in
            print("browseResultsChangedHandler")
            for result in results {
                //if case NWEndpoint.service = result.endpoint {
                    print("browser.browseResultsChangedHandler result: \(result)")
                //}
            }
        }
        browser.start(queue: .main)
        
    }
}

