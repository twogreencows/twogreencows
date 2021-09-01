//
//  ServersViewController.swift
//  Twogreencows
//
//  Created by Laurent Cerveau on 08/18/2021.
//

import UIKit

class ServersViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {

    let cellReuseIdentifier = "servers_cell"
    @IBOutlet var tableView: UITableView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        view.backgroundColor = UIColor.green
        tableView.backgroundColor = UIColor.clear
        tableView.delegate = self;
        tableView.dataSource = self;
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 3
    }
        
        // create a cell for each table view row
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        
    // create a new cell if needed or reuse an old one
        let  cell:UITableViewCell = self.tableView.dequeueReusableCell(withIdentifier: cellReuseIdentifier)!
    
            // set the text from the data model
        cell.textLabel?.text = "Server \(indexPath.row)"
        return cell

    }
}

