//
//  LocalViewController.swift
//  Twogreencows
//
//  Created by Laurent Cerveau on 08/25/2021.
//

import Foundation
import Network

import UIKit

class LocalViewController: UIViewController,UITableViewDelegate, UITableViewDataSource {

    let cellReuseIdentifier = "local_cell"
    @IBOutlet var tableView: UITableView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.view.backgroundColor = UIColor.blue
        tableView.backgroundColor = UIColor.clear
        tableView.delegate = self;
        tableView.dataSource = self;
        // Do any additional setup after loading the view.
    }

    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 4
    }
        
        // create a cell for each table view row
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        
    // create a new cell if needed or reuse an old one
        let  cell:UITableViewCell = self.tableView.dequeueReusableCell(withIdentifier: cellReuseIdentifier)!    
            // set the text from the data model
        cell.textLabel?.text = "Device \(indexPath.row)"
        return cell

    }
}

