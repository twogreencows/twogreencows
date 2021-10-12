//
//  AddServerViewController.swift
//  Twogreencows
//
//  Created by Laurent Cerveau on 10/11/2021.
//

import Foundation


import UIKit

class AddServerViewController: UIViewController {

    @IBOutlet var serverAddressField: UITextField!
    @IBOutlet var userNameField: UITextField!
    @IBOutlet var passwordField: UITextField!
    @IBOutlet var confirmPasswordField: UITextField!
    @IBOutlet var serverAddressLabel: UILabel!
    @IBOutlet var userNameLabel: UILabel!
    @IBOutlet var passwordLabel: UILabel!
    @IBOutlet var confirmPasswordLabel: UILabel!
    @IBOutlet var signActionButton: UIButton!
    @IBOutlet var changeSignModeButton: UIButton!
    
    var isSigninMode:Bool = true
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.isSigninMode = true
        confirmPasswordField.isHidden = true
        confirmPasswordLabel.isHidden = true
        // Do any additional setup after loading the view.
        
    }
        
        //will change the server mode to signup
    @IBAction func doSwitchSignMode(_ sender:UIButton) {
        if isSigninMode {
            confirmPasswordField.isHidden = false
            confirmPasswordLabel.isHidden = false
            changeSignModeButton.setTitle("Already an Account on this server ? Sign in!", for:.normal )
            signActionButton.setTitle("Sign up!", for:.normal )
            isSigninMode = false
        } else {
            confirmPasswordField.isHidden = true
            confirmPasswordLabel.isHidden = true
            changeSignModeButton.setTitle("No Account on this server:Sign up!", for:.normal )
            signActionButton.setTitle("Sign in!", for:.normal )
            isSigninMode = true
        }
    }
    
    @IBAction func doSignAction(_ sender:UIButton) {
        self.performSegue(withIdentifier: "add.backto.servers", sender: nil)
    }
}
