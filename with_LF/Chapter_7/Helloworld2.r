/* Helloworld2.r rez source for a simple generic application */

// the order of includes is important, since the last
// resources included will replace resources loaded
// before. We use this to override certain resources in 
// TApp and TDoc with the resources in Helloworld. 
include "TApp.rsrc";
include "TDoc.rsrc";
include "Helloworld2.rsrc";