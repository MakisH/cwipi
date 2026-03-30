---
title: 'CWIPI: Coupling With Interpolation Parallel Interface'
tags:
  - parallel computing
  - coupling
  - multi-physics
  - MPI

authors:
  - name: Eric Quémerais
    orcid: 0009-0007-2018-6358
    affiliation: "1"

  - name: Bastien Andrieu
    orcid: 0009-0000-9937-0244
    affiliation: "1"

  - name: Karmijn Hoogveld
    orcid: 0009-0007-8251-1152
    affiliation: "1"

affiliations:
 - index: 1
   name: DMPE, ONERA, Université Paris-Saclay, 92320 Châtillon, France

date: 27 january, 2026

bibliography: paper.bib

---

# Summary

CWIPI is an open-source library for the parallel coupling of independent simulation codes in multi-physics research. It provides MPI-based communication and fully distributed data exchange on non-coincident meshes, including on-the-fly spatial interpolation. Unlike other frameworks such as MUI and preCICE, CWIPI focuses on spatial interpolation and allows users to leverage solver-specific numerical methods, including finite volumes, finite elements, discontinuous Galerkin, and spectral methods. Advanced users can combine CWIPI’s geometric algorithms with solvers’ internal interpolation through callbacks, improving accuracy across heterogeneous solvers.

CWIPI supports a wide range of interface geometries, from linear surface and volumetric elements to higher-order elements with curved edges and faces. Performance-critical algorithms have been optimized for dynamic and adaptive meshes, and recent developments in the ParaDiGM library [@Quemerais2026] provide dynamic load and memory balancing for large-scale simulations. Ongoing work includes GPU acceleration of geometric algorithms [@Cazalbou2024] and the development of complementary temporal interpolation methods [@Simon2025], expanding CWIPI’s capabilities for complex, coupled multi-physics simulations.

# Statement of Need

Multi-physics problems can be approached either through monolithic methods, in which all physical phenomena are solved simultaneously within a single solver, or through partitioned methods, where independent solvers exchange data across shared interfaces. Partitioned approaches are particularly attractive in high-performance computing contexts, as they allow the reuse of existing specialized codes while maintaining their native parallel architecture.

CWIPI was initially developed in 2009 to provide an open-source solution complementary to MPCCI [@Joppich2006], whose black-box nature limited flexibility and transparency for multi-physics research. In this context, robust and efficient coupling tools are essential to enable data exchange between solvers operating on distinct meshes, geometries, and discretization schemes. Such tools must handle parallel communication, data distribution, and interpolation between non-coincident meshes while maintaining good performance and scalability.

CWIPI provides MPI-based communication, distributed data exchange, and a wide range of on-the-fly spatial interpolation methods. Unlike some existing frameworks such as MUI [@Tang2015] or preCICE [@Chourdakis2022], CWIPI focuses primarily on spatial interpolation, leaving temporal interpolation strategies to the user’s implementation.

Existing coupling tools often restrict supported interface geometries. For instance, MUI primarily supports point clouds, while preCICE supports point clouds or meshes composed of planar triangular or quadrilateral elements. CWIPI addresses these limitations by supporting a broad range of interface representations, including linear, surface and volumetric elements, polygons and polyhedra, as well as higher-order elements with curved edges and faces.

A key strength of CWIPI lies in its ability to **adapt to solvers’ own numerical methods** — including finite volumes, finite elements, discontinuous Galerkin, or spectral methods — and to leverage their internal interpolation capabilities. In an advanced mode, CWIPI exposes the results of its geometric algorithms, allowing users to combine them with the solver’s internal methods via a *callback*. For example, it is possible to use the functional bases of high-order elements from the source mesh while knowing the coordinates of target points from a finite-difference solver [@Leger2012], improving the accuracy of data exchange between heterogeneous solvers while maintaining consistency with each solver’s numerical scheme.

## Performance of Geometric Algorithms

A major challenge for CWIPI lies in the **performance of geometric algorithms**, especially when the coupling interface is moving during the simulation or when solvers employ dynamic mesh adaptation techniques. In these situations, geometric algorithms are invoked repeatedly during computation, requiring careful optimization.

In the 0.x branch of CWIPI, the algorithm relied on the FVM library from the computational fluid dynamics code Code_Saturne [@Archambeau2004], as well as on PLE, a lightweight coupling library [@Fournier2020]. To improve performance, new algorithms [@Andrieu2026] were developed within the ParaDiGM library [@Quemerais2026], originally created specifically to meet the needs of CWIPI. These algorithms provide **dynamic load and memory balancing** at each step, which is critical because coupling problems are inherently unbalanced. These developments bring a significant performance gain for large-scale and complex simulations.

## Ongoing Work / Future Directions

A major focus for CWIPI’s future development is the **GPU porting** of the most computationally expensive elements in the coupling phase, particularly the geometric algorithms. Recent research has led to the design of new algorithms [@Cazalbou2024], which will be integrated in an upcoming release.

The second research direction concerns the development of **temporal interpolation methods** [@Simon2025; @Francois2023] complementary to those provided by existing frameworks such as preCICE. These methods aim to improve the accuracy and flexibility of coupled multi-physics simulations, especially for dynamic interfaces and heterogeneous solvers.

# Mentions

From a scientific perspective, CWIPI is primarily used within the aeronautics and aerospace community from which it originated. The library is integrated into several software packages developed at ONERA, including elsA [@Cambier2013] for computational fluid dynamics, CEDRE [@Refloch2011] for multiphysics simulations, Z-set [@Garaud2019] for structural and materials mechanics, MoDeTheC [@Dellinger2024] for thermal degradation of materials, and SPACE [@Delorme2005] for acoustic propagation. Through these tools, CWIPI enables advanced multiphysics simulations in aerothermodynamics [@Errera2019], aeroacoustics [@Langenais2019], magnetohydrodynamics (MHD) [@Rocamora2025], electric arc propagation and fire safety [@Dellinger2023].

CWIPI is also employed as both an internal and external coupling library in leading combustion simulation environments such as YALES2 [@Moureau2011] and AVBP [@Schonfeld1999], where it supports efficient coupling between distinct physical models, meshes, and numerical resolutions.
A notable HPC application [@Dombard2018], involving a complex geometry and combining CWIPI with AVBP, demonstrated the robustness and scalability of the coupling approach on massively parallel architectures. Its capabilities had already been highlighted as early as 2017 [@Duchaine2017] and are widely recognized by the community, as evidenced by the invitation to the ExCALIBUR workshop [@Andrieu2021]. CWIPI has also enabled recent work in aeroelasticity with YALES2 [@Fabbri2023]. This technological maturity is reflected by CWIPI's integration into the software stack of national facilities such as the CCRT, confirming its recognition at a national level within large-scale high-performance computing environments.

Beyond its original ecosystem, the open-source community has also adopted CWIPI through GitHub-based interfacing projects, notably [@MoratillaVega2022; @nd922025] with OpenFOAM [@Weller1998] and Nektar++ [@Cantwell2015]. These developments have enabled applications in aeroacoustics as well as data assimilation strategies with OpenFOAM [@Valero2026], illustrating the openness, flexibility, and growing impact of CWIPI within the broader multiphysics and HPC communities. Finally, CWIPI serves as the interpolation engine of the OpenPALM coupler [@Duchaine2015], a higher-level reference coupling tool, demonstrating its central role in the ecosystem of multiphysics coupled simulations.

# Acknowledgements

The authors would like to thank the following people for their contributions to the development, testing, and dissemination of the CWIPI library within the community:

Romain Casta¹, Nicolas Dellinger², Florent Duchaine¹, Bastien Frisulli², Jean-Didier Garaud², Thomas Hennion², Stéphanie Lala², Xavier Lamboley¹², Bruno Maugars², Thierry Morel¹, Christophe Peyret²

¹ CERFACS
² ONERA

The authors also wish to thank BPI France, the Directorate General for Civil Aviation (DGAC), and the General Scientific Directorate of ONERA for their financial support.

# Author contributions

The contributions to this software are listed according to the CRediT taxonomy:

- **Eric Quémerais**: Conceptualization, Methodology, Software, Validation, Writing – Original Review & Editing, Project Administration, Funding Acquisition, Supervision
- **Bastien Andrieu**: Software, Validation, Writing – Original Draft
- **Karmijn Hoogveld**: Software, Validation, Writing – Original Draft

# References
