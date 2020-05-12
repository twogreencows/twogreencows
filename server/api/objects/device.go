/* A device has a greenhouse, and,  owner, an id, a posting key that rotates every 6 hours/10 request,
   a state reachable/unreachable an ip address and a hardware setup those are
     cameras: aperture/ pictures and such
     type: fixed, moving - inside/outside
     position : if moving mainly otherwise indicates
     sensors: temperature/hydro/pH - NKP

     Laurent Cerveau 2018

*/

package objects

type device struct {
	uuid            string
	greenhouse_uuid string
	owner_uuid      string
}
